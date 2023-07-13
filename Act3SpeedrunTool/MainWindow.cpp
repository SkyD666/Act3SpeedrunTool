#include "MainWindow.h"
#include "FirewallUtil.h"
#include "GlobalData.h"
#include "LogUtil.h"
#include "MemoryUtil.h"
#include "SettingDialog.h"
#include <MMSystem.h>
#include <QCoreApplication>
#include <QDateTime>
#include <QDesktopServices>
#include <QHotkey>
#include <QMessageBox>
#include <QPalette>
#include <QUrl>

const QString MainWindow::hotkeyStatePattern = "F: %1, %2  T: %3, %4, %5";

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , labCurrentHotkey(new QLabel(this))
    , labState(new QLabel(this))
{
    ui.setupUi(this);

    connect(qApp, &QCoreApplication::aboutToQuit, this, [=]() {
        GlobalData::destory();
    });

    FirewallUtil::init();

    setHotkey();

    initMenu();

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

    ui.btnStartFirewall->setText(tr("已关闭"));
    connect(ui.btnStartFirewall, &QAbstractButton::toggled, this, [=](bool checked) {
        if (checked == FirewallUtil::getIsEnabled()) {
            return;
        }
        bool succeed = FirewallUtil::setNetFwRuleEnabled(checked);
        if (succeed) {
            if (checked) {
                LogUtil::addLog("Firewall successfully enabled!");
                ui.btnStartFirewall->setText(tr("已开启"));
                QPalette palette = labState->palette();
                palette.setColor(QPalette::Window, Qt::green);
                labState->setPalette(palette);
                if (GlobalData::playSound) {
                    PlaySound(GlobalData::startSound.toStdWString().c_str(),
                        nullptr, SND_FILENAME | SND_ASYNC);
                }
            } else {
                LogUtil::addLog("Firewall successfully disabled!");
                ui.btnStartFirewall->setText(tr("已关闭"));
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
            ui.btnStartFirewall->setChecked(!checked);
        }
    });

    ui.btnStartFirewall->setFocus();

    connect(ui.btnStartHeadShot, &QAbstractButton::toggled, this, [=](bool checked) {
        if (checked) {
            if (startReadHeadShot()) {
                ui.btnStartHeadShot->setText(tr("点击关闭"));
            } else {
                ui.btnStartHeadShot->setChecked(false);
            }
        } else {
            stopReadHeadShot();
            ui.btnStartHeadShot->setText(tr("点击启动"));
        }
    });

    connect(ui.btnStartTimer, &QAbstractButton::toggled, this, [=](bool checked) {
        if (checked) {
            startTimer();
            ui.btnStartTimer->setText(tr("点击关闭"));
            ui.btnPauseTimer->setEnabled(true);
        } else {
            ui.btnPauseTimer->setChecked(false);
            stopTimer();
            ui.btnStartTimer->setText(tr("点击启动"));
            ui.btnPauseTimer->setEnabled(false);
        }
    });

    ui.btnPauseTimer->setEnabled(false);
    connect(ui.btnPauseTimer, &QAbstractButton::toggled, this, [=](bool checked) {
        if (checked) {
            pauseTimer();
            ui.btnPauseTimer->setText(tr("点击恢复"));
        } else {
            startTimer(true);
            ui.btnPauseTimer->setText(tr("点击暂停"));
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

void MainWindow::removeAllHotkeys()
{
    removeHotkey(startFirewallHotkey);
    removeHotkey(stopFirewallHotkey);
    removeHotkey(startTimerHotkey);
    removeHotkey(pauseTimerHotkey);
    removeHotkey(stopTimerHotkey);
}

void MainWindow::removeHotkey(QHotkey*& h)
{
    if (h) {
        h->setRegistered(false);
        h->disconnect();
        delete h;
        h = nullptr;
    }
}

void MainWindow::setHotkey()
{
    labCurrentHotkey->setText(hotkeyStatePattern.arg(
        GlobalData::startFirewallHotkey,
        GlobalData::stopFirewallHotkey,
        GlobalData::startTimerHotkey,
        GlobalData::pauseTimerHotkey,
        GlobalData::stopTimerHotkey));

    // 防火墙
    if (!GlobalData::startFirewallHotkey.isEmpty() && !GlobalData::stopFirewallHotkey.isEmpty()) {
        bool sameFirewallHotkey = GlobalData::startFirewallHotkey == GlobalData::stopFirewallHotkey;
        startFirewallHotkey = new QHotkey(QKeySequence(GlobalData::startFirewallHotkey), true, qApp);
        if (startFirewallHotkey->isRegistered()) {
            connect(startFirewallHotkey, &QHotkey::activated, qApp, [=]() {
                if (sameFirewallHotkey) {
                    ui.btnStartFirewall->toggle();
                } else {
                    ui.btnStartFirewall->setChecked(true);
                }
            });
        } else {
            QMessageBox::critical(nullptr, QString(), tr("注册启用防火墙热键失败！"));
        }
        if (!sameFirewallHotkey) {
            stopFirewallHotkey = new QHotkey(QKeySequence(GlobalData::stopFirewallHotkey), true, qApp);
            if (stopFirewallHotkey->isRegistered()) {
                connect(stopFirewallHotkey, &QHotkey::activated, qApp, [=]() {
                    ui.btnStartFirewall->setChecked(false);
                });
            } else {
                QMessageBox::critical(nullptr, QString(), tr("注册关闭防火墙热键失败！"));
            }
        }
    }

    // 计时器
    if (!GlobalData::startTimerHotkey.isEmpty() && !GlobalData::stopTimerHotkey.isEmpty()) {
        bool sameTimerHotkey = GlobalData::startTimerHotkey == GlobalData::stopTimerHotkey;
        startTimerHotkey = new QHotkey(QKeySequence(GlobalData::startTimerHotkey), true, qApp);
        if (startTimerHotkey->isRegistered()) {
            connect(startTimerHotkey, &QHotkey::activated, qApp, [=]() {
                if (sameTimerHotkey) {
                    ui.btnStartTimer->toggle();
                } else {
                    ui.btnStartTimer->setChecked(true);
                }
            });
        } else {
            QMessageBox::critical(nullptr, QString(), tr("注册启动计时器热键失败！"));
        }
        if (!sameTimerHotkey) {
            stopTimerHotkey = new QHotkey(QKeySequence(GlobalData::stopTimerHotkey), true, qApp);
            if (stopTimerHotkey->isRegistered()) {
                connect(stopTimerHotkey, &QHotkey::activated, qApp, [=]() {
                    ui.btnStartTimer->setChecked(false);
                });
            } else {
                QMessageBox::critical(nullptr, QString(), tr("注册停止计时器热键失败！"));
            }
        }
        if (GlobalData::pauseTimerHotkey == GlobalData::startTimerHotkey
            || GlobalData::pauseTimerHotkey == GlobalData::stopTimerHotkey) {
            QMessageBox::critical(nullptr, QString(), tr("暂停计时器热键与启动/停止计时器热键相同，暂停计时器热键将会无效！"));
        } else {
            pauseTimerHotkey = new QHotkey(QKeySequence(GlobalData::pauseTimerHotkey), true, qApp);
            if (pauseTimerHotkey->isRegistered()) {
                connect(pauseTimerHotkey, &QHotkey::activated, qApp, [=]() {
                    if (ui.btnPauseTimer->isEnabled()) {
                        ui.btnPauseTimer->toggle();
                    }
                });
            } else {
                QMessageBox::critical(nullptr, QString(), tr("注册暂停计时器热键失败！"));
            }
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
            showDisplayInfo();
        } else {
            if (displayInfoDialog) {
                displayInfoDialog->close();
                hideDisplayInfo();
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
        removeAllHotkeys();
        dialog->exec();
        setHotkey();
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

void MainWindow::showDisplayInfo()
{
    if (!topMostTimer) {
        topMostTimer = new QTimer(this);
    }
    connect(topMostTimer, &QTimer::timeout, this, [=]() {
        if (displayInfoDialogIsShowing && displayInfoDialog) {
            static RECT rect;
            HWND hwnd = (HWND)displayInfoDialog->winId();
            if (GetWindowRect(hwnd, &rect)) {
                SetWindowPos(hwnd, HWND_TOPMOST,
                    rect.left, rect.top,
                    rect.right - rect.left, rect.bottom - rect.top,
                    SWP_SHOWWINDOW);
            }
        }
    });
    topMostTimer->start(2000);
}

void MainWindow::hideDisplayInfo()
{
    if (topMostTimer) {
        topMostTimer->stop();
        delete topMostTimer;
        topMostTimer = nullptr;
    }
}

bool MainWindow::startReadHeadShot()
{
    if (!readMemTimer) {
        readMemTimer = new QTimer(this);
        gtaHandle = MemoryUtil::getProcessHandle(&pid);
        if (!gtaHandle) {
            QMessageBox::critical(nullptr, QString(), tr("获取窗口句柄失败！"));
            return false;
        }
    }
    int offsets[10] = { 0x30, 0x8, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x108, 0x3668 };
    connect(readMemTimer, &QTimer::timeout, this, [=]() {
        gtaHandle = MemoryUtil::getProcessHandle(&pid); // ?
        static short count = 0;
        static DWORD64 ptr;
        ReadProcessMemory(gtaHandle,
            (LPCVOID)((DWORD64)MemoryUtil::getProcessModuleHandle(pid, L"GTA5.exe") + 0x294E098),
            &ptr, sizeof(DWORD64), 0);
        for (int i = 0; i < sizeof(offsets) / sizeof(offsets[0]) - 1; i++) {
            ReadProcessMemory(gtaHandle, (LPCVOID)(ptr + offsets[i]), &ptr, sizeof(DWORD64), 0);
        }
        ReadProcessMemory(gtaHandle, (LPCVOID)(ptr + offsets[9]), &count, 2, 0);
        ui.labHeadShotCount->setText(QString::number(count));
        if (displayInfoDialogIsShowing && displayInfoDialog) {
            displayInfoDialog->setHeadShotCount(count);
        }
    });
    readMemTimer->start(200);

    return true;
}

void MainWindow::stopReadHeadShot()
{
    if (readMemTimer) {
        readMemTimer->stop();
        delete readMemTimer;
        readMemTimer = nullptr;
    }
    if (gtaHandle) {
        gtaHandle = NULL;
    }
    ui.labHeadShotCount->setText(tr("已停止记录"));
    if (displayInfoDialogIsShowing) {
        displayInfoDialog->setHeadShotCount(0);
    }
}

void MainWindow::startTimer(bool isContinue)
{
    if (!isContinue) {
        timerTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
    } else {
        timerTime += (QDateTime::currentDateTime().toMSecsSinceEpoch() - stoppedTime);
        stoppedTime = 0L;
    }
    if (!timer) {
        timer = new QTimer(this);
    }
    connect(timer, &QTimer::timeout, this, [=]() {
        long deltaTime = QDateTime::currentDateTime().toMSecsSinceEpoch() - timerTime;
        int m = deltaTime / 1000 / 60;
        int s = (deltaTime / 1000) % 60;
        int ms = (deltaTime % 1000) / 10;
        QString t = DisplayInfoDialog::timePattern
                        .arg("26")
                        .arg(m, 2, 10, QLatin1Char('0'))
                        .arg(s, 2, 10, QLatin1Char('0'))
                        .arg("16")
                        .arg(ms, 2, 10, QLatin1Char('0'));
        ui.labTimer->setText(t);
        if (displayInfoDialogIsShowing && displayInfoDialog) {
            displayInfoDialog->setTime(m, s, ms);
        }
    });
    timer->setTimerType(Qt::PreciseTimer);
    timer->start(50);
}

void MainWindow::pauseTimer()
{
    if (timer) {
        timer->stop();
        stoppedTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
    }
}

void MainWindow::stopTimer()
{
    if (timer) {
        timer->stop();
        timerTime = 0L;
        stoppedTime = 0L;
    }
}
