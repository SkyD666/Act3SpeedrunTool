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

    static QString hotkey;
    static bool playSound;

    static QString license;
};
