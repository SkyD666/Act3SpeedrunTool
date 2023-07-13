#include "GlobalData.h"
#include <QApplication>
#include <QColor>
#include <QFont>
#include <QSettings>
#include <windows.h>

QString GlobalData::startFirewallHotkey = "F9";
QString GlobalData::stopFirewallHotkey = "F9";
QString GlobalData::language = "";
QString GlobalData::startSound = "";
QString GlobalData::stopSound = "";
QString GlobalData::errorSound = "";
bool GlobalData::playSound = true;
bool GlobalData::displayInfoShow = true;
bool GlobalData::displayInfoTouchable = false;
Qt::Alignment GlobalData::displayInfoTextAlignment = Qt::AlignLeft | Qt::AlignVCenter;
QPoint GlobalData::displayInfoPos = { 20, 20 };
QSize GlobalData::displayInfoSize = { 300, 300 };
int GlobalData::displayInfoTextSize = 22;
QColor GlobalData::displayInfoTextColor = Qt::yellow;
QString GlobalData::displayInfoFontFamily = QFont().family();
QColor GlobalData::displayInfoBackground = Qt::transparent;
QString GlobalData::startTimerHotkey = "F7";
QString GlobalData::pauseTimerHotkey = "F8";
QString GlobalData::stopTimerHotkey = "F7";

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
    startFirewallHotkey = settings.value("Hotkey", startFirewallHotkey).toString();
    stopFirewallHotkey = settings.value("StopHotkey", stopFirewallHotkey).toString();
    language = settings.value("Language", "").toString();
    settings.endGroup();

    settings.beginGroup("Sound");
    startSound = settings.value("StartSound", "./sound/chimes.wav").toString();
    stopSound = settings.value("StopSound", "./sound/ding.wav").toString();
    errorSound = settings.value("ErrorSound", "./sound/error.wav").toString();
    playSound = settings.value("PlaySound", true).toBool();
    settings.endGroup();

    settings.beginGroup("DisplayInfo");
    RECT rect;
    rect.left = GetSystemMetrics(SM_XVIRTUALSCREEN);
    rect.top = GetSystemMetrics(SM_YVIRTUALSCREEN);
    rect.right = GetSystemMetrics(SM_CXVIRTUALSCREEN) + rect.left;
    rect.bottom = GetSystemMetrics(SM_CYVIRTUALSCREEN) + rect.top;
    displayInfoShow = settings.value("DisplayInfoShow", true).toBool();
    displayInfoTouchable = settings.value("DisplayInfoTouchable", true).toBool();
    displayInfoTextAlignment = Qt::Alignment(settings.value("DisplayInfoTextAlignment", displayInfoTextAlignment.toInt()).toInt());
    displayInfoSize = { qMax(qMin(settings.value("DisplayInfoWidth", 300).toInt(), rect.right - rect.left), 10),
        qMax(qMin(settings.value("DisplayInfoHeight", 300).toInt(), rect.bottom - rect.top), 10) };
    displayInfoPos = {
        qMax(qMin(settings.value("DisplayInfoPosX", 20).toInt(), rect.right),
            rect.left),
        qMax(qMin(settings.value("DisplayInfoPosY", 20).toInt(), rect.bottom),
            rect.top)
    };
    displayInfoTextSize = settings.value("DisplayInfoTextSize", displayInfoTextSize).toInt();
    displayInfoTextColor = settings.value("DisplayInfoTextColor", displayInfoTextColor.rgb()).toInt();
    displayInfoFontFamily = settings.value("DisplayInfoFontFamily", displayInfoFontFamily).toString();
    displayInfoBackground = QColor::fromRgba(settings.value("DisplayInfoBackground", displayInfoBackground.rgba()).toUInt());
    settings.endGroup();

    settings.beginGroup("Timer");
    startTimerHotkey = settings.value("StartTimerhotkey", startTimerHotkey).toString();
    pauseTimerHotkey = settings.value("PauseTimerHotkey", pauseTimerHotkey).toString();
    stopTimerHotkey = settings.value("StopTimerHotkey", stopTimerHotkey).toString();
    settings.endGroup();
}

void GlobalData::writeSettings()
{
    QSettings settings(getSettingsFilePath(), QSettings::IniFormat);

    settings.beginGroup("General");
    settings.setValue("Hotkey", startFirewallHotkey);
    settings.setValue("StopHotkey", stopFirewallHotkey);
    settings.setValue("Language", language);
    settings.endGroup();

    settings.beginGroup("Sound");
    settings.setValue("StartSound", startSound);
    settings.setValue("StopSound", stopSound);
    settings.setValue("ErrorSound", errorSound);
    settings.setValue("PlaySound", playSound);
    settings.endGroup();

    settings.beginGroup("DisplayInfo");
    settings.setValue("DisplayInfoShow", displayInfoShow);
    settings.setValue("DisplayInfoTouchable", displayInfoTouchable);
    settings.setValue("DisplayInfoTextAlignment", displayInfoTextAlignment.toInt());
    settings.setValue("DisplayInfoPosX", displayInfoPos.x());
    settings.setValue("DisplayInfoPosY", displayInfoPos.y());
    settings.setValue("DisplayInfoWidth", displayInfoSize.width());
    settings.setValue("DisplayInfoHeight", displayInfoSize.height());
    settings.setValue("DisplayInfoTextSize", displayInfoTextSize);
    settings.setValue("DisplayInfoTextColor", displayInfoTextColor.rgb());
    settings.setValue("DisplayInfoFontFamily", displayInfoFontFamily);
    settings.setValue("DisplayInfoBackground", displayInfoBackground.rgba());
    settings.endGroup();

    settings.beginGroup("Timer");
    settings.setValue("StartTimerhotkey", startTimerHotkey);
    settings.setValue("PauseTimerHotkey", pauseTimerHotkey);
    settings.setValue("StopTimerHotkey", stopTimerHotkey);
    settings.endGroup();
}
