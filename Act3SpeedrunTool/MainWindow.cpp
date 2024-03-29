#include "MainWindow.h"
#include "DescriptionDialog.h"
#include "DiscordUtil.h"
#include "FirewallUtil.h"
#include "GlobalData.h"
#include "HttpServerUtil.h"
#include "LogUtil.h"
#include "MemoryUtil.h"
#include "SettingDialog.h"
#include "TimeUtil.h"
#include "UpdateDialog.h"
#include <MMSystem.h>
#include <QBoxLayout>
#include <QClipboard>
#include <QCloseEvent>
#include <QCoreApplication>
#include <QDateTime>
#include <QDesktopServices>
#include <QFile>
#include <QHotkey>
#include <QMessageBox>
#include <QPalette>
#include <QState>
#include <QUrl>

const QString MainWindow::hotkeyStatePattern = "F: %1, %2  T: %3, %4, %5  X: %6";

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , labCurrentHotkey(new QLabel(this))
    , labState(new QLabel(this))
{
    ui.setupUi(this);

    connect(qApp, &QCoreApplication::aboutToQuit, this, [=]() {
        HttpServerController::instance()->stop();
        qInfo("Exiting...");
        globalData->destory();
    });

    initSystemTray();

    discordUtil->initDiscord();

    FirewallUtil::init();

    setHotkey();

    initMenu();

    checkUpdate();

    ui.statusbar->addPermanentWidget(labCurrentHotkey);

    labState->setAutoFillBackground(true);
    labState->setMinimumWidth(16);
    QPalette palette = labState->palette();
    palette.setColor(QPalette::Window, Qt::red);
    labState->setPalette(palette);
    ui.statusbar->addPermanentWidget(labState);

    initFirewall();

    connect(ui.btnStartHeadShot, &QAbstractButton::toggled, this, [this](bool checked) {
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

    initTimerStateMachine();

    initCloseGameImmediately();

    initGlobalDataConnects();
}

MainWindow::~MainWindow()
{
    FirewallUtil::release();
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    if (!waitingToExit && globalData->minimizeToTray()) {
        setVisible(false);
        event->ignore();
        qInfo("MainWindow minimized to tray");
        return;
    } else {
        if (displayInfoDialog) {
            displayInfoDialog->done(-1);
        }
    }

    if (event->isAccepted()) {
        qApp->exit();
    }
}

void MainWindow::initGlobalDataConnects()
{
    connect(globalData, &GlobalData::minimizeToTrayChanged, this, [this]() {
        if (globalData->minimizeToTray()) {
            initSystemTray();
        } else {
            closeSystemTray();
        }
    });
    connect(globalData, &GlobalData::headshotUpdateIntervalChanged, this, [this]() {
        if (headShotTimer) {
            headShotTimer->setInterval(globalData->headshotUpdateInterval());
        }
    });
    connect(globalData, &GlobalData::timerUpdateIntervalChanged, this, [this]() {
        if (timer) {
            timer->setInterval(globalData->timerUpdateInterval());
        }
    });
}

void MainWindow::checkUpdate()
{
    if (!globalData->autoCheckUpdate()) {
        return;
    }
    UpdateDialog::sendCheckRequest(
        this,
        [this](GitHubRelease* release) {
            if (UpdateDialog::isNewVersion(release->tagName)
                && globalData->ignoredNewVersion() != release->tagName) {
                (new UpdateDialog(release, this))->exec();
            } else {
                delete release;
            }
        },
        [](int, QString) {});
}

void MainWindow::removeAllHotkeys()
{
    removeHotkey(startFirewallHotkey);
    removeHotkey(stopFirewallHotkey);
    removeHotkey(startTimerHotkey);
    removeHotkey(pauseTimerHotkey);
    removeHotkey(stopTimerHotkey);
    removeHotkey(closeGameImmediatelyHotkey);
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
        globalData->firewallStartHotkey(),
        globalData->firewallStopHotkey(),
        globalData->timerStartHotkey(),
        globalData->timerPauseHotkey(),
        globalData->timerStopHotkey(),
        globalData->closeGameImmediatelyHotkey()));

    // 防火墙
    if (!globalData->firewallStartHotkey().isEmpty() && !globalData->firewallStopHotkey().isEmpty()) {
        bool sameFirewallHotkey = globalData->firewallStartHotkey() == globalData->firewallStopHotkey();
        startFirewallHotkey = new QHotkey(QKeySequence(globalData->firewallStartHotkey()), true, qApp);
        if (startFirewallHotkey->isRegistered()) {
            connect(startFirewallHotkey, &QHotkey::activated, qApp, [this, sameFirewallHotkey]() {
                if (sameFirewallHotkey) {
                    ui.btnStartFirewall->toggle();
                } else {
                    ui.btnStartFirewall->setChecked(true);
                }
            });
        } else {
            QMessageBox::critical(this, QString(), tr("注册启用防火墙热键失败！"));
        }
        if (!sameFirewallHotkey) {
            stopFirewallHotkey = new QHotkey(QKeySequence(globalData->firewallStopHotkey()), true, qApp);
            if (stopFirewallHotkey->isRegistered()) {
                connect(stopFirewallHotkey, &QHotkey::activated, qApp, [this]() {
                    ui.btnStartFirewall->setChecked(false);
                });
            } else {
                QMessageBox::critical(this, QString(), tr("注册关闭防火墙热键失败！"));
            }
        }
    }

    // 计时器
    if (!globalData->timerStartHotkey().isEmpty() && !globalData->timerStopHotkey().isEmpty()) {
        bool sameTimerHotkey = globalData->timerStartHotkey() == globalData->timerStopHotkey();
        startTimerHotkey = new QHotkey(QKeySequence(globalData->timerStartHotkey()), true, qApp);
        if (startTimerHotkey->isRegistered()) {
            connect(startTimerHotkey, &QHotkey::activated, qApp, [this, sameTimerHotkey]() {
                if (sameTimerHotkey) {
                    ui.btnStartTimer->click();
                } else {
                    if (globalData->timerStopStrategy() == TimerStopStrategy::StopSecondZero) {
                        // 在 stoppedState 时，按下开始热键要求不能开始，因此try尝试
                        // 只有 stoppedAndZeroState 接受 tryToTimerRunningState 信号
                        // 因此 TimerStopStrategy::StopSecondZero 且在 stoppedState 时按下开始热键也没有接收者
                        emit tryToTimerRunningState();
                    } else {
                        emit toTimerRunningState();
                    }
                }
            });
        } else {
            QMessageBox::critical(nullptr, QString(), tr("注册启动计时器热键失败！"));
        }
        if (!sameTimerHotkey) {
            stopTimerHotkey = new QHotkey(QKeySequence(globalData->timerStopHotkey()), true, qApp);
            if (stopTimerHotkey->isRegistered()) {
                connect(stopTimerHotkey, &QHotkey::activated, qApp, [=]() {
                    if (globalData->timerStopStrategy() == TimerStopStrategy::OnlyStop
                        || globalData->timerStopStrategy() == TimerStopStrategy::StopAndZero) {
                        // 在 stoppedState 时，按下停止热键要求不能（再次）归零，因此try尝试
                        // 只有 runningState 接受 tryToTimerStoppedOrStoppedAndZeroState 信号
                        // 因此 (TimerStopStrategy::OnlyStop 或 TimerStopStrategy::StopAndZero) 且在 stoppedState 时按下停止热键也没有接收者
                        emit tryToTimerStoppedOrStoppedAndZeroState();
                    } else {
                        emit toTimerStoppedOrStoppedAndZeroState();
                    }
                });
            } else {
                QMessageBox::critical(this, QString(), tr("注册停止计时器热键失败！"));
            }
        }
        if (globalData->timerPauseHotkey() == globalData->timerStartHotkey()
            || globalData->timerPauseHotkey() == globalData->timerStopHotkey()) {
            QMessageBox::critical(this, QString(), tr("暂停计时器热键与启动/停止计时器热键相同，暂停计时器热键将会无效！"));
        } else {
            pauseTimerHotkey = new QHotkey(QKeySequence(globalData->timerPauseHotkey()), true, qApp);
            if (pauseTimerHotkey->isRegistered()) {
                connect(pauseTimerHotkey, &QHotkey::activated, qApp, [this]() {
                    if (ui.btnPauseTimer->isEnabled()) {
                        ui.btnPauseTimer->click();
                    }
                });
            } else {
                QMessageBox::critical(this, QString(), tr("注册暂停计时器热键失败！"));
            }
        }
    }

    // 快速结束游戏
    if (!globalData->closeGameImmediatelyHotkey().isEmpty()) {
        closeGameImmediatelyHotkey = new QHotkey(QKeySequence(globalData->closeGameImmediatelyHotkey()), true, qApp);
        if (closeGameImmediatelyHotkey->isRegistered()) {
            connect(closeGameImmediatelyHotkey, &QHotkey::activated, qApp, [this]() {
                closeGameImmediately();
            });
        } else {
            QMessageBox::critical(this, QString(), tr("注册快速结束游戏热键失败！"));
        }
    }
}

void MainWindow::initMenu()
{
    connect(ui.actionExit, &QAction::triggered, this, [this]() {
        waitingToExit = true;
        close();
    });
    connect(ui.actionShow, &QAction::triggered, this, [this]() {
        show();
        setWindowState(Qt::WindowActive);
    });
    connect(ui.actionDisplayInfo, &QAction::toggled, this, [=](bool checked) {
        static bool firstTime = true;
        if (!firstTime && globalData->displayInfoShow() == checked) {
            return;
        }
        if (firstTime) {
            firstTime = false;
        }
        globalData->setDisplayInfoShow(checked);
        if (checked) {
            displayInfoDialog = new DisplayInfoDialog();
            auto closeLambda = [this](int result) {
                disconnect(this, nullptr, displayInfoDialog, nullptr);
                displayInfoDialogIsShowing = false;
                // Qt::WA_DeleteOnClose
                // delete displayInfoDialog;
                displayInfoDialog = nullptr;
                // -1表示不需要设置globalData->displayInfo = false也不需要setChecked(false)的情况
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
    ui.actionDisplayInfo->setChecked(globalData->displayInfoShow());

    connect(ui.actionDisplayInfoTouchable, &QAction::toggled, this, [this](bool checked) {
        globalData->setDisplayInfoTouchable(checked);
        // 重启页面
        if (ui.actionDisplayInfo->isChecked()) {
            ui.actionDisplayInfo->setChecked(false);
            ui.actionDisplayInfo->setChecked(true);
        }
    });
    ui.actionDisplayInfoTouchable->setChecked(globalData->displayInfoTouchable());

    // 启动服务器
    auto enableServerLambda = [=](bool checked) {
        globalData->setDisplayInfoServer(checked);
        ui.actionServerCopyHostAddress->setEnabled(checked);
        ui.actionServerOpenBrowser->setEnabled(checked);
        if (checked) {
            ui.actionServerCopyHostAddress->setText(tr("复制地址"));
            HttpServerController::instance()->start();
        } else {
            ui.actionServerCopyHostAddress->setText(tr("服务器未运行"));
            HttpServerController::instance()->stop();
        }
    };
    connect(ui.actionEnableServer, &QAction::toggled, this, enableServerLambda);
    ui.actionEnableServer->setChecked(globalData->displayInfoServer());
    enableServerLambda(globalData->displayInfoServer());

    connect(ui.actionServerCopyHostAddress, &QAction::triggered, this, [this]() {
        auto domain = HttpServerUtil::getHttpServerDomain();
        if (domain.isEmpty()) {
            QMessageBox::critical(this, QString(), tr("获取服务器地址失败！"));
        } else {
            QGuiApplication::clipboard()->setText(domain);
            QMessageBox::information(this, QString(), tr("已复制服务器地址！"));
        }
    });

    connect(ui.actionServerOpenBrowser, &QAction::triggered, this, [this]() {
        auto domain = HttpServerUtil::getHttpServerDomain();
        if (domain.isEmpty()) {
            QMessageBox::critical(this, QString(), tr("获取服务器地址失败！"));
        } else {
            QDesktopServices::openUrl(QUrl(domain));
        }
    });

    connect(ui.actionSetting, &QAction::triggered, this, [this]() {
        auto dialog = new SettingDialog(this);
        removeAllHotkeys();
        dialog->exec();
        setHotkey();
    });

    connect(ui.actionUpdate, &QAction::triggered, this, [this]() {
        (new UpdateDialog(nullptr, this))->exec();
    });

    connect(ui.actionDescription, &QAction::triggered, this, [this]() {
        (new DescriptionDialog(this))->exec();
    });

    connect(ui.actionLogDir, &QAction::triggered, this, []() {
        QDesktopServices::openUrl(QUrl("file:///" + LogUtil::getLogDir()));
    });

    connect(ui.actionLog, &QAction::triggered, this, []() {
        QDesktopServices::openUrl(QUrl("file:///" + LogUtil::getLogFilePath()));
    });

    connect(ui.actionGitHub, &QAction::triggered, this, []() {
        QDesktopServices::openUrl(QUrl("https://github.com/SkyD666/Act3SpeedrunTool"));
    });

    connect(ui.actionAiFaDian, &QAction::triggered, this, []() {
        QDesktopServices::openUrl(QUrl("https://afdian.net/a/SkyD666"));
    });
    connect(ui.actionBuyMeACoffee, &QAction::triggered, this, []() {
        QDesktopServices::openUrl(QUrl("https://www.buymeacoffee.com/SkyD666"));
    });
    connect(ui.actionAliPay, &QAction::triggered, this, [this]() {
        QDialog dialog(this);
        QLabel* imageLabel = new QLabel(&dialog);
        imageLabel->setPixmap(QPixmap("://image/ic_alipay.jpg"));
        auto layout = new QBoxLayout(QBoxLayout::LeftToRight);
        layout->addWidget(imageLabel);
        layout->setSizeConstraint(QLayout::SetFixedSize);
        dialog.setLayout(layout);
        dialog.setWindowTitle(tr("支付宝收款二维码"));
        dialog.exec();
    });

    connect(ui.actionHelpTranslate, &QAction::triggered, this, []() {
        QDesktopServices::openUrl(QUrl("https://discord.gg/pEWEjeJTa3"));
    });

    connect(ui.actionAcknowledgment, &QAction::triggered, this, [this]() {
        QMessageBox::information(this, tr("致谢"), acknowledgment);
    });

    connect(ui.actionAboutQt, &QAction::triggered, this, []() { qApp->aboutQt(); });

    connect(ui.actionAbout, &QAction::triggered, this, [this]() {
        QMessageBox::about(this, QString(), license);
    });
}

void MainWindow::initFirewall()
{
    connect(ui.pbFirewallRefreshState, &QAbstractButton::clicked, this, [=]() {
        bool firewallIsEnabled = FirewallUtil::firewallIsEnabled();
        ui.btnStartFirewall->setEnabled(firewallIsEnabled);
        ui.btnStartFirewall->setDisabled(!firewallIsEnabled);
        ui.btnStartFirewall->setCheckable(firewallIsEnabled);
        ui.groupBoxFirewallState->setVisible(!firewallIsEnabled);
    });

    connect(ui.pbFirewallOpenSysWindow, &QAbstractButton::clicked, this, [=]() {
        ShellExecute(NULL, L"open", L"control.exe", L"firewall.cpl", NULL, SW_SHOWNORMAL);
    });

    ui.pbFirewallRefreshState->click();

    ui.cbSound->setChecked(globalData->firewallPlaySound());
    connect(ui.cbSound, &QCheckBox::stateChanged, this, [](int state) {
        globalData->setFirewallPlaySound(state);
    });

    ui.btnStartFirewall->setText(tr("已关闭"));
    connect(ui.btnStartFirewall, &QAbstractButton::toggled, this, [this](bool checked) {
        if (checked == FirewallUtil::getIsEnabled()) {
            return;
        }
        bool succeed = FirewallUtil::setNetFwRuleEnabled(checked);
        if (succeed) {
            if (checked) {
                ui.btnStartFirewall->setText(tr("已开启"));
                QPalette palette = labState->palette();
                palette.setColor(QPalette::Window, Qt::green);
                labState->setPalette(palette);
                if (globalData->firewallPlaySound()) {
                    PlaySound(globalData->firewallStartSound().toStdWString().c_str(),
                        nullptr, SND_FILENAME | SND_ASYNC);
                }
            } else {
                ui.btnStartFirewall->setText(tr("已关闭"));
                QPalette palette = labState->palette();
                palette.setColor(QPalette::Window, Qt::red);
                labState->setPalette(palette);
                if (globalData->firewallPlaySound()) {
                    PlaySound(globalData->firewallStopSound().toStdWString().c_str(),
                        nullptr, SND_FILENAME | SND_ASYNC);
                }
            }
        } else {
            if (globalData->firewallPlaySound()) {
                PlaySound(globalData->firewallErrorSound().toStdWString().c_str(),
                    nullptr, SND_FILENAME | SND_ASYNC);
            }
            ui.btnStartFirewall->setChecked(!checked);
        }
    });

    ui.btnStartFirewall->setFocus();
}

void MainWindow::initCloseGameImmediately()
{
    ui.labCloseGameImmediatelyHotkey->setText(globalData->closeGameImmediatelyHotkey());
    connect(globalData, &GlobalData::closeGameImmediatelyHotkeyChanged, this, [this]() {
        ui.labCloseGameImmediatelyHotkey->setText(globalData->closeGameImmediatelyHotkey());
    });
    connect(ui.btnCloseGameImmediately, &QAbstractButton::toggled, this, [this]() {
        closeGameImmediately();
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
            //            HttpServerController::instance()->sendNewData(QTime::currentTime().second());
        }
    });
    topMostTimer->start(3000);
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
    if (!headShotTimer) {
        headShotTimer = new QTimer(this);
        gtaHandle = MemoryUtil::getProcessHandle(&pid);
        if (!gtaHandle) {
            QMessageBox::critical(nullptr, QString(), tr("获取窗口句柄失败，未检测到 GTA5，请启动或重启游戏后再进行尝试！"));
            return false;
        }
    }
    int offsets[10] = { 0x30, 0x8, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x108, 0x3668 };
    static bool firstTime = true;
    connect(headShotTimer, &QTimer::timeout, this, [=]() {
        if (gtaHandle) {
            // 关闭上次的 OpenProcess
            CloseHandle(gtaHandle);
        }
        gtaHandle = MemoryUtil::getProcessHandle(&pid); // 必须每次获取数据时都刷新，否则re后获取不到爆头数
        static DWORD64 ptr;
        static short count = 0;
        ReadProcessMemory(gtaHandle,
            (LPCVOID)((DWORD64)MemoryUtil::getProcessModuleHandle(pid, L"GTA5.exe") + 0x2FDF678),
            &ptr, sizeof(DWORD64), 0);
        for (int i = 0; i < sizeof(offsets) / sizeof(offsets[0]) - 1; i++) {
            ReadProcessMemory(gtaHandle, (LPCVOID)(ptr + offsets[i]), &ptr, sizeof(DWORD64), 0);
        }
        ReadProcessMemory(gtaHandle, (LPCVOID)(ptr + offsets[9]), &count, 2, 0);
        if (firstTime || count != headshotCount) {
            headshotCount = count;
            HttpServerController::instance()->sendNewData(headshotCount);
            ui.labHeadShotCount->setText(QString::number(headshotCount));
            if (displayInfoDialogIsShowing && displayInfoDialog) {
                displayInfoDialog->setHeadShotCount(headshotCount);
            }
            discordUtil->setHeadshotCount(headshotCount);
        }
        firstTime = false;
    });
    firstTime = true;
    headShotTimer->start(globalData->headshotUpdateInterval());

    return true;
}

void MainWindow::stopReadHeadShot()
{
    if (headShotTimer) {
        headShotTimer->stop();
        delete headShotTimer;
        headShotTimer = nullptr;
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
        timerTime += (QDateTime::currentDateTime().toMSecsSinceEpoch() - pausedTime);
        pausedTime = 0L;
    }
    if (!timer) {
        timer = new QTimer(this);
    }
    connect(timer, &QTimer::timeout, this, [this]() {
        updateTimerString();
    });
    timer->setTimerType(Qt::PreciseTimer);
    timer->start(globalData->timerUpdateInterval());
    HttpServerController::instance()->startOrContinueTimer(isContinue, timerTime);
}

void MainWindow::pauseTimer()
{
    if (timer) {
        timer->stop();
        pausedTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
        updateTimerString(pausedTime);
        HttpServerController::instance()->pauseTimer(pausedTime);
    }
}

void MainWindow::stopTimer()
{
    if (timer) {
        if (timer->isActive()) {
            timer->stop();
        }
        qint64 stoppedTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
        updateTimerString(stoppedTime);
        HttpServerController::instance()->stopTimer(stoppedTime);
        timerTime = 0L;
        pausedTime = 0L;
    }
}

void MainWindow::zeroTimer()
{
    if (displayInfoDialogIsShowing && displayInfoDialog) {
        displayInfoDialog->setTime(0, 0, 0);
    }
    ui.labTimer->setText(DisplayInfoDialog::timePattern.arg("26", "00", "00", "16", "00"));
    HttpServerController::instance()->zeroTimer();
    discordUtil->setSpeedrunTime(0, 0);
}

void MainWindow::updateTimerString(qint64 currentDateTime)
{
    qint64 deltaTime = currentDateTime - timerTime;
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
    discordUtil->setSpeedrunTime(m, s);
}

void MainWindow::initTimerStateMachine()
{
    // 设置 parent 后相当于已经 addState 了
    QState* stoppedAndZeroState = new QState(&timerStateMachine);
    // 判断到停止并归零 还是 运行态
    QState* stoppedAndZeroOrRunningState = new QState(&timerStateMachine);
    QState* stoppedState = new QState(&timerStateMachine);
    QState* runningState = new QState(&timerStateMachine);
    QState* pausedState = new QState(&timerStateMachine);

    stoppedAndZeroState->addTransition(ui.btnStartTimer, &QAbstractButton::clicked, runningState);
    stoppedAndZeroState->addTransition(this, &MainWindow::toTimerRunningState, runningState);
    stoppedAndZeroState->addTransition(this, &MainWindow::tryToTimerRunningState, runningState);

    runningState->addTransition(ui.btnStartTimer, &QAbstractButton::clicked, stoppedState);
    runningState->addTransition(ui.btnPauseTimer, &QAbstractButton::clicked, pausedState);
    runningState->addTransition(this, &MainWindow::toTimerStoppedOrStoppedAndZeroState, stoppedState);
    runningState->addTransition(this, &MainWindow::tryToTimerStoppedOrStoppedAndZeroState, stoppedState);

    stoppedState->addTransition(ui.btnStartTimer, &QAbstractButton::clicked, stoppedAndZeroOrRunningState);
    stoppedState->addTransition(this, &MainWindow::toTimerRunningState, runningState);
    stoppedState->addTransition(this, &MainWindow::toTimerStoppedAndZeroState, stoppedAndZeroState);
    stoppedState->addTransition(this, &MainWindow::toTimerStoppedOrStoppedAndZeroState, stoppedAndZeroState);

    stoppedAndZeroOrRunningState->addTransition(this, &MainWindow::toTimerStoppedAndZeroState, stoppedAndZeroState);
    stoppedAndZeroOrRunningState->addTransition(this, &MainWindow::toTimerRunningState, runningState);

    pausedState->addTransition(ui.btnStartTimer, &QAbstractButton::clicked, stoppedAndZeroState);
    pausedState->addTransition(ui.btnPauseTimer, &QAbstractButton::clicked, runningState);
    pausedState->addTransition(this, &MainWindow::toTimerStoppedOrStoppedAndZeroState, stoppedAndZeroState);

    connect(stoppedAndZeroOrRunningState, &QState::entered, this, [=]() {
        switch (globalData->timerStopStrategy()) {
        // 停止后不归零（v3.3方案）
        case TimerStopStrategy::OnlyStop:
            emit toTimerRunningState();
            break;
        // 第二次按停止键归零
        case TimerStopStrategy::StopSecondZero:
            emit toTimerStoppedAndZeroState();
            break;
        default:
            emit toTimerRunningState();
            break;
        }
    });
    connect(stoppedAndZeroState, &QState::entered, this, [=]() {
        ui.btnStartTimer->setText(tr("点击启动"));
        ui.btnPauseTimer->setEnabled(false);
        ui.btnPauseTimer->setText(tr("点击暂停"));
        stopTimer();
        zeroTimer();
    });
    connect(stoppedAndZeroState, &QState::exited, this, [=]() {
        startTimer(false);
    });
    connect(stoppedState, &QState::exited, this, [=]() {
        // 注意是 != 不能改成 == OnlyStop
        // 因为有种情况：在当前状态，调整了策略为 StopSecondZero，为了让点击开始能够 startTimer，只能用 !=
        if (globalData->timerStopStrategy() != TimerStopStrategy::StopSecondZero) { // 停止后不归零（v3.3方案）
            startTimer(false);
        }
    });
    connect(stoppedState, &QState::entered, this, [=]() {
        ui.btnPauseTimer->setEnabled(false);
        ui.btnPauseTimer->setText(tr("点击暂停"));
        switch (globalData->timerStopStrategy()) {
        // 停止后不归零（v3.3方案）
        case TimerStopStrategy::OnlyStop:
            ui.btnStartTimer->setText(tr("点击启动"));
            break;
        // 停止后立即归零
        case TimerStopStrategy::StopAndZero:
            emit toTimerStoppedAndZeroState();
            break;
        // 第二次按停止键归零
        case TimerStopStrategy::StopSecondZero:
            ui.btnStartTimer->setText(tr("点击归零"));
            break;
        }
        stopTimer();
    });
    connect(runningState, &QState::entered, this, [=]() {
        ui.btnStartTimer->setText(tr("点击停止"));
        ui.btnPauseTimer->setEnabled(true);
        ui.btnPauseTimer->setText(tr("点击暂停"));
    });
    connect(pausedState, &QState::entered, this, [=]() {
        ui.btnPauseTimer->setText(tr("点击恢复"));
        pauseTimer();
    });
    connect(pausedState, &QState::exited, this, [=]() {
        startTimer(true);
    });

    timerStateMachine.setInitialState(stoppedAndZeroState);
    timerStateMachine.start();
}

void MainWindow::closeGameImmediately()
{
    DWORD p;
    const auto game = MemoryUtil::getProcessHandle(&p, PROCESS_TERMINATE);
    TerminateProcess(game, 1);
    CloseHandle(game);
    //    QThread* workerThread = new QThread(this);
    //    connect(workerThread, &QThread::started, workerThread, []() {

    //    });
    //    connect(workerThread, &QThread::finished, workerThread, &QObject::deleteLater);
    //    workerThread->start();

    //    PlaySound(globalData->firewallStartSound().toStdWString().c_str(),
    //        nullptr, SND_FILENAME | SND_ASYNC);
    //    keybd_event('S', MapVirtualKey('S', 0), 0, 0);
    //    TimeUtil::nanosleep(20);
    //    keybd_event('D', MapVirtualKey('D', 0), 0, 0);
    //    TimeUtil::nanosleep(20);
    //    keybd_event(VK_SPACE, MapVirtualKey(VK_SPACE, 0), 0, 0);
    //    TimeUtil::nanosleep(20);
    //    keybd_event(VK_SPACE, MapVirtualKey(VK_SPACE, 0), KEYEVENTF_KEYUP, 0);
    //    TimeUtil::nanosleep(100);
    //    keybd_event('S', MapVirtualKey('S', 0), KEYEVENTF_KEYUP, 0);
    //    TimeUtil::nanosleep(100);
    //    keybd_event('D', MapVirtualKey('D', 0), KEYEVENTF_KEYUP, 0);
}

void MainWindow::initSystemTray()
{
    if (!globalData->minimizeToTray() || systemTray) {
        return;
    }
    systemTrayMenu = new QMenu(this);
    systemTray = new QSystemTrayIcon(QApplication::windowIcon(), this);

    systemTrayMenu->addAction(ui.actionShow);
    systemTrayMenu->addSeparator();
    systemTrayMenu->addAction(ui.actionSetting);
    systemTrayMenu->addAction(ui.actionSetting);
    systemTrayMenu->addMenu(ui.menuDisplayInfo);
    systemTrayMenu->addSeparator();
    systemTrayMenu->addAction(ui.actionExit);

    systemTray->setToolTip(QApplication::applicationName());
    systemTray->setContextMenu(systemTrayMenu);

    connect(systemTray, &QSystemTrayIcon::activated, this, [=](QSystemTrayIcon::ActivationReason reason) {
        switch (reason) {
        case QSystemTrayIcon::Context:
            systemTrayMenu->show();
            break;
        case QSystemTrayIcon::DoubleClick:
        case QSystemTrayIcon::Trigger:
        case QSystemTrayIcon::MiddleClick:
            ui.actionShow->trigger();
            qInfo("Tray clicked");
            break;
        case QSystemTrayIcon::Unknown:
            break;
        }
    });

    systemTray->show();
}

void MainWindow::closeSystemTray()
{
    if (!systemTray) {
        return;
    }
    systemTray->hide();
    delete systemTray;
    delete systemTrayMenu;
    systemTray = nullptr;
    systemTrayMenu = nullptr;
    setVisible(true);
}
