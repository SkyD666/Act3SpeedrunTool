#include "GlobalData.h"
#include <QApplication>
#include <QColor>
#include <QFont>
#include <QSettings>
#include <windows.h>

QList<SubFunction> GlobalData::funcs = { SubFunction::Firewall, SubFunction::Headshot, SubFunction::Timer };
QString GlobalData::firewallStartHotkey = "F9";
QString GlobalData::firewallStopHotkey = "F9";
QString GlobalData::language = "";
QString GlobalData::firewallStartSound = "";
QString GlobalData::firewallStopSound = "";
QString GlobalData::firewallErrorSound = "";
bool GlobalData::firewallPlaySound = true;
QString GlobalData::firewallAppPath = "";
QSubFuncSettingsMap GlobalData::subFunctionSettings;
bool GlobalData::displayInfoShow = true;
bool GlobalData::displayInfoServer = false;
bool GlobalData::displayInfoTouchable = false;
QPoint GlobalData::displayInfoPos = { 20, 20 };
QSize GlobalData::displayInfoSize = { 300, 100 };
QColor GlobalData::displayInfoBackground = Qt::transparent;
QString GlobalData::startTimerHotkey = "F7";
QString GlobalData::pauseTimerHotkey = "F8";
QString GlobalData::stopTimerHotkey = "F7";
bool GlobalData::timerZeroAfterStop = false;

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
    language = settings.value("Language", "").toString();
    settings.endGroup();

    settings.beginGroup("DisplayInfo");
    RECT rect;
    rect.left = GetSystemMetrics(SM_XVIRTUALSCREEN);
    rect.top = GetSystemMetrics(SM_YVIRTUALSCREEN);
    rect.right = GetSystemMetrics(SM_CXVIRTUALSCREEN) + rect.left;
    rect.bottom = GetSystemMetrics(SM_CYVIRTUALSCREEN) + rect.top;
    displayInfoShow = settings.value("DisplayInfoShow", true).toBool();
    displayInfoTouchable = settings.value("DisplayInfoTouchable", true).toBool();
    displayInfoServer = settings.value("DisplayInfoServer", displayInfoServer).toBool();
    displayInfoSize = {
        qMax(qMin(settings.value("DisplayInfoWidth", displayInfoSize.width()).toInt(), rect.right - rect.left), 10),
        qMax(qMin(settings.value("DisplayInfoHeight", displayInfoSize.height()).toInt(), rect.bottom - rect.top), 10)
    };
    displayInfoPos = {
        qMax(qMin(settings.value("DisplayInfoPosX", 20).toInt(), rect.right), rect.left),
        qMax(qMin(settings.value("DisplayInfoPosY", 20).toInt(), rect.bottom), rect.top)
    };
    displayInfoBackground = QColor::fromRgba(settings.value("DisplayInfoBackground", displayInfoBackground.rgba()).toUInt());
    settings.endGroup();

    settings.beginGroup("Timer");
    startTimerHotkey = settings.value("StartTimerhotkey", startTimerHotkey).toString();
    pauseTimerHotkey = settings.value("PauseTimerHotkey", pauseTimerHotkey).toString();
    stopTimerHotkey = settings.value("StopTimerHotkey", stopTimerHotkey).toString();
    timerZeroAfterStop = settings.value("TimerZeroAfterStop", timerZeroAfterStop).toBool();
    settings.endGroup();

    settings.beginGroup("Firewall");
    firewallStartHotkey = settings.value("FirewallStartHotkey", firewallStartHotkey).toString();
    firewallStopHotkey = settings.value("FirewallStopHotkey", firewallStopHotkey).toString();
    firewallStartSound = settings.value("FirewallStartSound", "./sound/chimes.wav").toString();
    firewallStopSound = settings.value("FirewallStopSound", "./sound/ding.wav").toString();
    firewallErrorSound = settings.value("FirewallErrorSound", "./sound/error.wav").toString();
    firewallPlaySound = settings.value("FirewallPlaySound", firewallPlaySound).toBool();
    firewallAppPath = settings.value("FirewallAppPath", firewallAppPath).toString();
    settings.endGroup();

    readSubFuncSettingsMap(settings);
}

void GlobalData::writeSettings()
{
    QSettings settings(getSettingsFilePath(), QSettings::IniFormat);

    settings.beginGroup("General");
    settings.setValue("Language", language);
    settings.endGroup();

    settings.beginGroup("DisplayInfo");
    settings.setValue("DisplayInfoShow", displayInfoShow);
    settings.setValue("DisplayInfoTouchable", displayInfoTouchable);
    settings.setValue("DisplayInfoServer", displayInfoServer);
    settings.setValue("DisplayInfoPosX", displayInfoPos.x());
    settings.setValue("DisplayInfoPosY", displayInfoPos.y());
    settings.setValue("DisplayInfoWidth", displayInfoSize.width());
    settings.setValue("DisplayInfoHeight", displayInfoSize.height());
    settings.setValue("DisplayInfoBackground", displayInfoBackground.rgba());
    settings.endGroup();

    settings.beginGroup("Timer");
    settings.setValue("StartTimerhotkey", startTimerHotkey);
    settings.setValue("PauseTimerHotkey", pauseTimerHotkey);
    settings.setValue("StopTimerHotkey", stopTimerHotkey);
    settings.setValue("TimerZeroAfterStop", timerZeroAfterStop);
    settings.endGroup();

    settings.beginGroup("Firewall");
    settings.setValue("FirewallStartHotkey", firewallStartHotkey);
    settings.setValue("FirewallStopHotkey", firewallStopHotkey);
    settings.setValue("FirewallStartSound", firewallStartSound);
    settings.setValue("FirewallStopSound", firewallStopSound);
    settings.setValue("FirewallErrorSound", firewallErrorSound);
    settings.setValue("FirewallPlaySound", firewallPlaySound);
    settings.setValue("FirewallAppPath", firewallAppPath);
    settings.endGroup();

    writeSubFuncSettingsMap(settings);
}

void GlobalData::readSubFuncSettingsMap(QSettings& settings)
{
    QSubFuncSettingsMap subFunctionSettings;
    for (auto i : funcs) {
        subFunctionSettings[i] = SubFunctionSettingItem();
    }
    settings.beginGroup("SubFunctions");
    for (auto i = subFunctionSettings.constBegin(); i != subFunctionSettings.constEnd(); i++) {
        auto defaultValue = i.value();
        settings.beginGroup(SubFunctionUtil::toString(i.key()));
        defaultValue.display = settings.value("Display", defaultValue.display).toBool();
        defaultValue.updateIntervalMs = settings.value("UpdateIntervalMs", defaultValue.updateIntervalMs).toInt();
        defaultValue.textAlignment = Qt::Alignment(
            settings.value("TextAlignment", defaultValue.textAlignment.toInt()).toInt());
        defaultValue.textSize = settings.value("TextSize", defaultValue.textSize).toInt();
        defaultValue.textColor = settings.value("TextColor", defaultValue.textColor).value<QColor>();
        defaultValue.textShadowColor = settings.value("TextShadowColor", defaultValue.textShadowColor)
                                           .value<QColor>();
        defaultValue.textShadowBlurRadius = settings.value("TextShadowBlurRadius", defaultValue.textShadowBlurRadius).toInt();
        defaultValue.textShadowOffset = settings.value("TextShadowOffset", defaultValue.textShadowOffset).value<QPointF>();
        defaultValue.fontFamily = settings.value("FontFamily", defaultValue.fontFamily).toString();
        GlobalData::subFunctionSettings[i.key()] = defaultValue;
        settings.endGroup();
    }
    settings.endGroup();
}

void GlobalData::writeSubFuncSettingsMap(QSettings& settings)
{
    settings.beginGroup("SubFunctions");
    for (auto i = subFunctionSettings.constBegin(); i != subFunctionSettings.constEnd(); i++) {
        auto currentValue = i.value();
        settings.beginGroup(SubFunctionUtil::toString(i.key()));
        settings.setValue("Display", currentValue.display);
        settings.setValue("UpdateIntervalMs", currentValue.updateIntervalMs);
        settings.setValue("TextAlignment", currentValue.textAlignment.toInt());
        settings.setValue("TextSize", currentValue.textSize);
        settings.setValue("TextColor", currentValue.textColor);
        settings.setValue("TextShadowColor", currentValue.textShadowColor);
        settings.setValue("TextShadowBlurRadius", currentValue.textShadowBlurRadius);
        settings.setValue("TextShadowOffset", currentValue.textShadowOffset);
        settings.setValue("FontFamily", currentValue.fontFamily);
        settings.endGroup();
    }
    settings.endGroup();
}
