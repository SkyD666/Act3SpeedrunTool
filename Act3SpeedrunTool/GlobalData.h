#include "SubFunction.h"
#include <QColor>
#include <QMetaType>
#include <QPoint>
#include <QSettings>
#include <QSize>
#include <QString>

#pragma once

typedef QMap<SubFunction, SubFunctionSettingItem> QSubFuncSettingsMap;

class GlobalData {
public:
    GlobalData();

    static QList<SubFunction> funcs;

    static inline QString getSettingsFilePath();

    static void init();
    static void destory();

    static void readSettings();

    static void writeSettings();

    static QString startFirewallHotkey;
    static QString stopFirewallHotkey;
    static QString language;
    static bool playSound;

    static QSubFuncSettingsMap subFunctionSettings;
    static bool displayInfoShow;
    static bool displayInfoTouchable;
    static QPoint displayInfoPos;
    static QSize displayInfoSize;
    static QColor displayInfoBackground;
    static QString startTimerHotkey;
    static QString pauseTimerHotkey;
    static QString stopTimerHotkey;

    static QString startSound;
    static QString stopSound;
    static QString errorSound;

private:
    static void readSubFuncSettingsMap(QSettings& settings);
    static void writeSubFuncSettingsMap(QSettings& settings);
};
