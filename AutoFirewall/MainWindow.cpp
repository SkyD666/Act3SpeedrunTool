#include "MainWindow.h"
#include "FirewallUtil.h"
#include "GlobalData.h"
#include "LogUtil.h"
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

    connect(ui.actionSetting, &QAction::triggered, this, [=]() {
        auto dialog = new SettingDialog(this);
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

    connect(ui.actionAboutQt, &QAction::triggered, this, [=]() { qApp->aboutQt(); });

    connect(ui.actionAbout, &QAction::triggered, this, [=]() {
        QMessageBox::about(this, QString(), license);
    });

    ui.btnEnable->setFocus();
}

MainWindow::~MainWindow()
{
    FirewallUtil::release();
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
