#include "GlobalData.h"
#include <QApplication>
#include <QSettings>

QString GlobalData::hotkey = "F9";
QString GlobalData::language = "";
QString GlobalData::startSound = "";
QString GlobalData::stopSound = "";
QString GlobalData::errorSound = "";
bool GlobalData::playSound = true;

GlobalData::GlobalData()
{
}

QString GlobalData::getSettingsFilePath()
{
    return QCoreApplication::applicationDirPath() + "/Config.ini";
}

void GlobalData::init()
{
    readSettings();
}

void GlobalData::destory()
{
    writeSettings();
}

void GlobalData::readSettings()
{
    QSettings settings(getSettingsFilePath(), QSettings::IniFormat);

    settings.beginGroup("General");
    hotkey = settings.value("Hotkey", "F9").toString();
    language = settings.value("Language", "").toString();
    settings.endGroup();

    settings.beginGroup("Sound");
    startSound = settings.value("StartSound", "./sound/chimes.wav").toString();
    stopSound = settings.value("StopSound", "./sound/ding.wav").toString();
    errorSound = settings.value("ErrorSound", "./sound/error.wav").toString();
    playSound = settings.value("PlaySound", true).toBool();
    settings.endGroup();
}

void GlobalData::writeSettings()
{
    QSettings settings(getSettingsFilePath(), QSettings::IniFormat);

    settings.beginGroup("General");
    settings.setValue("Hotkey", hotkey);
    settings.setValue("Language", language);
    settings.endGroup();

    settings.beginGroup("Sound");
    settings.setValue("StartSound", startSound);
    settings.setValue("StopSound", stopSound);
    settings.setValue("ErrorSound", errorSound);
    settings.setValue("PlaySound", playSound);
    settings.endGroup();
}
