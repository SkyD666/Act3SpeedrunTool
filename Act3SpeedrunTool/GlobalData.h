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

    static QString firewallStartHotkey;
    static QString firewallStopHotkey;
    static QString language;
    static bool firewallPlaySound;

    static QString firewallAppPath;

    static QSubFuncSettingsMap subFunctionSettings;
    static bool displayInfoShow;
    static bool displayInfoTouchable;
    static bool displayInfoServer;
    static QPoint displayInfoPos;
    static QSize displayInfoSize;
    static QColor displayInfoBackground;
    static QString startTimerHotkey;
    static QString pauseTimerHotkey;
    static QString stopTimerHotkey;
    static bool timerZeroAfterStop;

    static QString firewallStartSound;
    static QString firewallStopSound;
    static QString firewallErrorSound;

private:
    static void readSubFuncSettingsMap(QSettings& settings);
    static void writeSubFuncSettingsMap(QSettings& settings);
};
