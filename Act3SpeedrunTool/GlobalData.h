#include <QColor>
#include <QPoint>
#include <QSize>
#include <QString>

#pragma once

class GlobalData {
public:
    GlobalData();

    static inline QString getSettingsFilePath();

    static void init();
    static void destory();

    static void readSettings();

    static void writeSettings();

    static QString startFirewallHotkey;
    static QString stopFirewallHotkey;
    static QString language;
    static bool playSound;
    static bool displayInfoShow;
    static bool displayInfoTouchable;
    static Qt::Alignment displayInfoTextAlignment;
    static QPoint displayInfoPos;
    static QSize displayInfoSize;
    static int displayInfoTextSize;
    static QColor displayInfoTextColor;
    static QString displayInfoFontFamily;
    static QColor displayInfoBackground;
    static QString startTimerHotkey;
    static QString pauseTimerHotkey;
    static QString stopTimerHotkey;

    static QString startSound;
    static QString stopSound;
    static QString errorSound;
};
