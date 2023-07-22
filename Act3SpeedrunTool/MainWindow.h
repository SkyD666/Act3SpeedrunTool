#pragma once

#include "DisplayInfoDialog.h"
#include "ui_MainWindow.h"
#include <QHotkey>
#include <QLabel>
#include <QMainWindow>
#include <QStateMachine>
#include <QTimer>
#include <windows.h>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent* event) override;

    void removeAllHotkeys();

    void removeHotkey(QHotkey*& h);

    void setHotkey();

    void updateTimerInterval();

    void initMenu();

    void showDisplayInfo();

    void hideDisplayInfo();

    bool startReadHeadShot();

    void stopReadHeadShot();

    void startTimer(bool isContinue = false);

    void pauseTimer();

    void stopTimer();

    void zeroTimer();

    void initTimerStateMachine();

signals:
    void toTimerStoppedAndZeroState();
    void toTimerStoppedOrStoppedAndZeroState();
    void toTimerRunningState();
    void tryToTimerRunningState();
    void tryToTimerStoppedOrStoppedAndZeroState();

private:
    Ui::MainWindow ui;

    static const QString hotkeyStatePattern;

    bool displayInfoDialogIsShowing = false;
    DisplayInfoDialog* displayInfoDialog = nullptr;

    QHotkey* startFirewallHotkey = nullptr;
    QHotkey* stopFirewallHotkey = nullptr;

    QHotkey* startTimerHotkey = nullptr;
    QHotkey* pauseTimerHotkey = nullptr;
    QHotkey* stopTimerHotkey = nullptr;

    QLabel* labCurrentHotkey;

    QLabel* labState;

    short headshotCount = 0;
    QTimer* headShotTimer = nullptr;
    QTimer* topMostTimer = nullptr;

    QTimer* timer = nullptr;
    qint64 stoppedTime = 0L; // 上次暂停的时间
    qint64 timerTime = 0L; // 计时器开始的时间
    QStateMachine timerStateMachine = QStateMachine(this); // 计时器状态

    DWORD pid = 0;

    HANDLE gtaHandle = NULL;

    QString license = "<h3>" + QApplication::applicationDisplayName()
        + " v" + QApplication::applicationVersion() + "</h3>"
        + tr("<p>一个 GTAOL 末日三速通工具，支持快速启动防火墙、自动统计爆头数、手动计时。本软件仅读取内存，不会修改任何内存。</p>")
        + "<p>" + tr("作者：") + QApplication::organizationName() + "</p>"
        + "<p><a href='https://discord.gg/pEWEjeJTa3'>" + tr("加入 Discord 一起划水~") + "</a></p>"
        + "<p><a href='https://afdian.net/a/SkyD666'>" + tr("赞助作者") + "</a></p>"
        + "<p>" + tr("特别感谢 <b>ACT3 帮会</b>对本工具的宣传和支持。特别感谢 <b>CZ9946 和 Adstargod</b> 两位大佬，他们对本工具的研发提供了大量的帮助！") + "</p>"
        + "<p>" + tr("感谢所有提供赞助的朋友们！") + "</p>"
        + QString("<p>Copyright <font>&#169;</font> 2023 SkyD666</p>"
                  R"(<p>This program is free software: you can redistribute it and/or modify
                     it under the terms of the GNU General Public License as published by
                     the Free Software Foundation, either version 3 of the License, or
                     (at your option) any later version.</p>
                     <p>This program is distributed in the hope that it will be useful,
                     but WITHOUT ANY WARRANTY; without even the implied warranty of
                     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
                     GNU General Public License for more details.</p>
                     <p>You should have received a copy of the GNU General Public License
                     along with this program. If not, see
                     &lt;<a href='https://www.gnu.org/licenses/'>https://www.gnu.org/licenses/</a>&gt;.</p>)");
};
