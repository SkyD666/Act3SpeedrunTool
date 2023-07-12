#include "MainWindow.h"
#include "FirewallUtil.h"
#include "GlobalData.h"
#include "LogUtil.h"
#include "MemoryUtil.h"
#include "SettingDialog.h"
#include <MMSystem.h>
#include <QCoreApplication>
#include <QDesktopServices>
#include <QHotkey>
#include <QMessageBox>
#include <QPalette>
#include <QUrl>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , hotkey(nullptr)
    , labCurrentHotkey(new QLabel(this))
    , labState(new QLabel(this))
{
    ui.setupUi(this);

    connect(qApp, &QCoreApplication::aboutToQuit, this, [=]() {
        GlobalData::destory();
    });

    FirewallUtil::init();

    setHotkey(GlobalData::hotkey, GlobalData::stopHotkey);

    initMenu();

    labCurrentHotkey->setText(GlobalData::hotkey + ", " + GlobalData::stopHotkey);
    ui.statusbar->addPermanentWidget(labCurrentHotkey);

    labState->setAutoFillBackground(true);
    labState->setMinimumWidth(16);
    QPalette palette = labState->palette();
    palette.setColor(QPalette::Window, Qt::red);
    labState->setPalette(palette);
    ui.statusbar->addPermanentWidget(labState);

    ui.cbSound->setChecked(GlobalData::playSound);
    connect(ui.cbSound, &QCheckBox::stateChanged, this, [=](int state) {
        GlobalData::playSound = state;
    });

    ui.btnEnable->setText(tr("已关闭"));
    connect(ui.btnEnable, &QAbstractButton::toggled, this, [=](bool checked) {
        if (checked == FirewallUtil::getIsEnabled()) {
            return;
        }
        bool succeed = FirewallUtil::setNetFwRuleEnabled(checked);
        if (succeed) {
            if (checked) {
                LogUtil::addLog("Firewall successfully enabled!");
                ui.btnEnable->setText(tr("已开启"));
                QPalette palette = labState->palette();
                palette.setColor(QPalette::Window, Qt::green);
                labState->setPalette(palette);
                if (GlobalData::playSound) {
                    PlaySound(GlobalData::startSound.toStdWString().c_str(),
                        nullptr, SND_FILENAME | SND_ASYNC);
                }
            } else {
                LogUtil::addLog("Firewall successfully disabled!");
                ui.btnEnable->setText(tr("已关闭"));
                QPalette palette = labState->palette();
                palette.setColor(QPalette::Window, Qt::red);
                labState->setPalette(palette);
                if (GlobalData::playSound) {
                    PlaySound(GlobalData::stopSound.toStdWString().c_str(),
                        nullptr, SND_FILENAME | SND_ASYNC);
                }
            }
        } else {
            LogUtil::addLog("Firewall operate failed!");
            if (GlobalData::playSound) {
                PlaySound(GlobalData::errorSound.toStdWString().c_str(),
                    nullptr, SND_FILENAME | SND_ASYNC);
            }
            ui.btnEnable->setChecked(!checked);
        }
    });

    ui.btnEnable->setFocus();

    connect(ui.btnStartHeadShot, &QAbstractButton::toggled, this, [=](bool checked) {
        if (checked) {
            startReadHeadShot();
            ui.btnStartHeadShot->setText(tr("点击关闭"));
        } else {
            stopReadHeadShot();
            ui.btnStartHeadShot->setText(tr("点击启动"));
        }
    });
}

MainWindow::~MainWindow()
{
    FirewallUtil::release();
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    if (displayInfoDialog) {
        displayInfoDialog->done(-1);
    }
}

void MainWindow::removeHotkey()
{
    if (!hotkey) {
        return;
    }
    hotkey->setRegistered(false);
    hotkey->disconnect();
    delete hotkey;
    hotkey = nullptr;
}

void MainWindow::setHotkey(const QString hotkeyStr, const QString hotkeyStopStr)
{
    labCurrentHotkey->setText(hotkeyStr + ", " + hotkeyStopStr);
    if (hotkeyStr.isEmpty() || hotkeyStopStr.isEmpty()) {
        return;
    }
    bool sameHotkey = hotkeyStr == hotkeyStopStr;
    hotkey = new QHotkey(QKeySequence(hotkeyStr), true, qApp);
    if (hotkey->isRegistered()) {
        connect(hotkey, &QHotkey::activated, qApp, [=]() {
            if (sameHotkey) {
                ui.btnEnable->toggle();
            } else {
                ui.btnEnable->setChecked(true);
            }
        });
    } else {
        QMessageBox::critical(nullptr, QString(), tr("热键注册失败！"));
    }

    if (!sameHotkey) {
        hotkeyStop = new QHotkey(QKeySequence(hotkeyStopStr), true, qApp);
        if (hotkeyStop->isRegistered()) {
            connect(hotkeyStop, &QHotkey::activated, qApp, [=]() {
                ui.btnEnable->setChecked(false);
            });
        } else {
            QMessageBox::critical(nullptr, QString(), tr("热键注册失败！"));
        }
    }
}

void MainWindow::initMenu()
{
    connect(ui.actionDisplayInfo, &QAction::toggled, this, [=](bool checked) {
        static bool firstTime = true;
        if (!firstTime && GlobalData::displayInfoShow == checked) {
            return;
        }
        if (firstTime) {
            firstTime = false;
        }
        GlobalData::displayInfoShow = checked;
        if (checked) {
            displayInfoDialog = new DisplayInfoDialog();
            auto closeLambda = [=](int result) {
                disconnect(this, nullptr, displayInfoDialog, nullptr);
                displayInfoDialogIsShowing = false;
                // Qt::WA_DeleteOnClose
                // delete displayInfoDialog;
                displayInfoDialog = nullptr;
                // -1表示不需要设置GlobalData::displayInfo = false也不需要setChecked(false)的情况
                if (result != -1) {
                    ui.actionDisplayInfo->setChecked(false);
                }
            };
            connect(displayInfoDialog, &DisplayInfoDialog::finished, this, closeLambda);
            displayInfoDialog->show();
            // TODO：加锁？
            displayInfoDialogIsShowing = true;
        } else {
            if (displayInfoDialog) {
                displayInfoDialog->close();
            }
        }
    });
    ui.actionDisplayInfo->setChecked(GlobalData::displayInfoShow);

    connect(ui.actionDisplayInfoTouchable, &QAction::toggled, this, [=](bool checked) {
        GlobalData::displayInfoTouchable = checked;
        // 重启页面
        if (ui.actionDisplayInfo->isChecked()) {
            ui.actionDisplayInfo->setChecked(false);
            ui.actionDisplayInfo->setChecked(true);
        }
    });
    ui.actionDisplayInfoTouchable->setChecked(GlobalData::displayInfoTouchable);

    connect(ui.actionSetting, &QAction::triggered, this, [=]() {
        auto dialog = new SettingDialog(this, displayInfoDialog);
        removeHotkey();
        dialog->exec();
        setHotkey(GlobalData::hotkey, GlobalData::stopHotkey);
    });

    connect(ui.actionLogDir, &QAction::triggered, this, [=]() {
        QDesktopServices::openUrl(QUrl("file:///" + LogUtil::getLogDir()));
    });

    connect(ui.actionLog, &QAction::triggered, this, [=]() {
        QDesktopServices::openUrl(QUrl("file:///" + LogUtil::getLogFilePath()));
    });

    connect(ui.actionGitHub, &QAction::triggered, this, [=]() {
        QDesktopServices::openUrl(QUrl("https://github.com/SkyD666/AutoFirewall"));
    });

    connect(ui.actionSponsor, &QAction::triggered, this, [=]() {
        QDesktopServices::openUrl(QUrl("https://afdian.net/a/SkyD666"));
    });

    connect(ui.actionAboutQt, &QAction::triggered, this, [=]() { qApp->aboutQt(); });

    connect(ui.actionAbout, &QAction::triggered, this, [=]() {
        QMessageBox::about(this, QString(), license);
    });
}

void MainWindow::startReadHeadShot()
{
    if (!timer) {
        timer = new QTimer(this);
        gtaHandle = MemoryUtil::getProcessHandle(&pid);
        if (!gtaHandle) {
            QMessageBox::critical(nullptr, QString(), tr("获取窗口句柄失败！"));
        }
    }
    int offsets[10] = { 0x28, 0x8, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x108, 0x3668 };
    connect(timer, &QTimer::timeout, this, [=]() {
        short count = 0;
        DWORD64 ptr;
        ReadProcessMemory(gtaHandle,
            (LPCVOID)((DWORD64)MemoryUtil::getProcessModuleHandle(pid, L"GTA5.exe") + 0x29A0278),
            &ptr, sizeof(DWORD64), 0);
        for (int i = 0; i < sizeof(offsets) / sizeof(offsets[0]) - 1; i++) {
            ReadProcessMemory(gtaHandle, (LPCVOID)(ptr + offsets[i]), &ptr, sizeof(DWORD64), 0);
        }
        ReadProcessMemory(gtaHandle, (LPCVOID)(ptr + offsets[9]), &count, 2, 0);
        ui.labHeadShotCount->setText(QString::number(count));
        if (displayInfoDialogIsShowing) {
            displayInfoDialog->setHeadShotCount(count);
            RECT rect;
            HWND hwnd = (HWND)displayInfoDialog->winId();
            if (GetWindowRect(hwnd, &rect)) {
                SetWindowPos(hwnd, HWND_TOPMOST,
                    rect.left, rect.top,
                    rect.right - rect.left, rect.bottom - rect.top,
                    SWP_SHOWWINDOW);
            }
        }
    });
    timer->start(200);
}

void MainWindow::stopReadHeadShot()
{
    if (timer) {
        timer->stop();
        delete timer;
        timer = nullptr;
    }
    if (gtaHandle) {
        gtaHandle = NULL;
    }
    ui.labHeadShotCount->setText(tr("已停止记录"));
    if (displayInfoDialogIsShowing) {
        displayInfoDialog->setHeadShotCount(0);
    }
}
