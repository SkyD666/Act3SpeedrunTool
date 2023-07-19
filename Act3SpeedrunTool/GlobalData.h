#include "DisplayInfoSubFunction.h"
#include <QColor>
#include <QMetaType>
#include <QPoint>
#include <QSettings>
#include <QSize>
#include <QString>

#pragma once

typedef QMap<DisplayInfoSubFunction, DisplayInfoSubFunctionItem> QDisplayInfoSubFuncsMap;

enum TimerStopStrategy {
    OnlyStop,
    StopAndZero,
    StopSecondZero
};

namespace TimerStopStrategyUtil {
TimerStopStrategy fromString(QString strategy);
QString toDisplayString(TimerStopStrategy strategy);
QString toString(TimerStopStrategy strategy);
}

class GlobalData {
public:
    GlobalData();

    static inline QString getSettingsFilePath();

    static void init();
    static void destory();

    static void readSettings();

    static void writeSettings();

    // 信息展示
    static QList<DisplayInfoSubFunction> funcs;
    static QDisplayInfoSubFuncsMap displayInfoSubFunctions;
    static bool displayInfoShow;
    static bool displayInfoTouchable;
    static bool displayInfoServer;
    static QPoint displayInfoPos;
    static QSize displayInfoSize;
    static QColor displayInfoBackground;

    // 防火墙
    static QString firewallStartHotkey;
    static QString firewallStopHotkey;
    static QString firewallStartSound;
    static QString firewallStopSound;
    static QString firewallErrorSound;
    static bool firewallPlaySound;
    static QString firewallAppPath;

    // 爆头
    static int headshotUpdateInterval;

    // 计时器
    static QString timerStartHotkey;
    static QString timerPauseHotkey;
    static QString timerStopHotkey;
    static QList<TimerStopStrategy> timerStopStrategies;
    static TimerStopStrategy timerStopStrategy;
    static int timerUpdateInterval;

    // 语言
    static QString language;

private:
    // 读取/写入信息展示设置
    static void readSubFuncSettingsMap(QSettings& settings);
    static void writeSubFuncSettingsMap(QSettings& settings);
};
