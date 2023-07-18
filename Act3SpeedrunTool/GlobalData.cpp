#include "GlobalData.h"
#include <QApplication>
#include <QColor>
#include <QFont>
#include <QSettings>
#include <windows.h>

// 信息展示
QList<DisplayInfoSubFunction> GlobalData::funcs = { DisplayInfoSubFunction::Firewall, DisplayInfoSubFunction::Headshot, DisplayInfoSubFunction::Timer };
QDisplayInfoSubFuncsMap GlobalData::displayInfoSubFunctions;
bool GlobalData::displayInfoShow = true;
bool GlobalData::displayInfoServer = false;
bool GlobalData::displayInfoTouchable = false;
QPoint GlobalData::displayInfoPos = { 20, 20 };
QSize GlobalData::displayInfoSize = { 300, 100 };
QColor GlobalData::displayInfoBackground = Qt::transparent;
// 防火墙
QString GlobalData::firewallStartHotkey = "F9";
QString GlobalData::firewallStopHotkey = "F9";
QString GlobalData::firewallStartSound = "";
QString GlobalData::firewallStopSound = "";
QString GlobalData::firewallErrorSound = "";
bool GlobalData::firewallPlaySound = true;
QString GlobalData::firewallAppPath = "";
// 爆头
int GlobalData::headshotUpdateInterval = 100;
// 计时器
QString GlobalData::timerStartHotkey = "F7";
QString GlobalData::timerPauseHotkey = "F8";
QString GlobalData::timerStopHotkey = "F7";
bool GlobalData::timerZeroAfterStop = false;
int GlobalData::timerUpdateInterval = 50;
// 语言
QString GlobalData::language = "";

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
    readSubFuncSettingsMap(settings);

    settings.beginGroup("Firewall");
    firewallStartHotkey = settings.value("FirewallStartHotkey", firewallStartHotkey).toString();
    firewallStopHotkey = settings.value("FirewallStopHotkey", firewallStopHotkey).toString();
    firewallStartSound = settings.value("FirewallStartSound", "./sound/chimes.wav").toString();
    firewallStopSound = settings.value("FirewallStopSound", "./sound/ding.wav").toString();
    firewallErrorSound = settings.value("FirewallErrorSound", "./sound/error.wav").toString();
    firewallPlaySound = settings.value("FirewallPlaySound", firewallPlaySound).toBool();
    firewallAppPath = settings.value("FirewallAppPath", firewallAppPath).toString();
    settings.endGroup();

    settings.beginGroup("Headshot");
    headshotUpdateInterval = settings.value("HeadshotUpdateInterval", headshotUpdateInterval).toInt();
    settings.endGroup();

    settings.beginGroup("Timer");
    timerStartHotkey = settings.value("TimerStarthotkey", timerStartHotkey).toString();
    timerPauseHotkey = settings.value("TimerPauseHotkey", timerPauseHotkey).toString();
    timerStopHotkey = settings.value("TimerStopHotkey", timerStopHotkey).toString();
    timerZeroAfterStop = settings.value("TimerZeroAfterStop", timerZeroAfterStop).toBool();
    timerUpdateInterval = settings.value("TimerUpdateInterval", timerUpdateInterval).toInt();
    settings.endGroup();

    settings.beginGroup("Language");
    language = settings.value("Language", "").toString();
    settings.endGroup();
}

void GlobalData::writeSettings()
{
    QSettings settings(getSettingsFilePath(), QSettings::IniFormat);

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
    writeSubFuncSettingsMap(settings);

    settings.beginGroup("Firewall");
    settings.setValue("FirewallStartHotkey", firewallStartHotkey);
    settings.setValue("FirewallStopHotkey", firewallStopHotkey);
    settings.setValue("FirewallStartSound", firewallStartSound);
    settings.setValue("FirewallStopSound", firewallStopSound);
    settings.setValue("FirewallErrorSound", firewallErrorSound);
    settings.setValue("FirewallPlaySound", firewallPlaySound);
    settings.setValue("FirewallAppPath", firewallAppPath);
    settings.endGroup();

    settings.beginGroup("Headshot");
    settings.setValue("HeadshotUpdateInterval", headshotUpdateInterval);
    settings.endGroup();

    settings.beginGroup("Timer");
    settings.setValue("TimerStarthotkey", timerStartHotkey);
    settings.setValue("TimerPauseHotkey", timerPauseHotkey);
    settings.setValue("TimerStopHotkey", timerStopHotkey);
    settings.setValue("TimerZeroAfterStop", timerZeroAfterStop);
    settings.setValue("TimerUpdateInterval", timerUpdateInterval);
    settings.endGroup();

    settings.beginGroup("Language");
    settings.setValue("Language", language);
    settings.endGroup();
}

void GlobalData::readSubFuncSettingsMap(QSettings& settings)
{
    QDisplayInfoSubFuncsMap displayInfoSubFunctions;
    for (auto i : funcs) {
        displayInfoSubFunctions[i] = DisplayInfoSubFunctionItem();
    }
    settings.beginGroup("SubFunctions");
    for (auto i = displayInfoSubFunctions.constBegin(); i != displayInfoSubFunctions.constEnd(); i++) {
        auto defaultValue = i.value();
        settings.beginGroup(DisplayInfoSubFunctionUtil::toString(i.key()));
        defaultValue.display = settings.value("Display", defaultValue.display).toBool();
        defaultValue.textAlignment = Qt::Alignment(
            settings.value("TextAlignment", defaultValue.textAlignment.toInt()).toInt());
        defaultValue.textSize = settings.value("TextSize", defaultValue.textSize).toInt();
        defaultValue.textColor = settings.value("TextColor", defaultValue.textColor).value<QColor>();
        defaultValue.textShadowColor = settings.value("TextShadowColor", defaultValue.textShadowColor)
                                           .value<QColor>();
        defaultValue.textShadowBlurRadius = settings.value("TextShadowBlurRadius", defaultValue.textShadowBlurRadius).toInt();
        defaultValue.textShadowOffset = settings.value("TextShadowOffset", defaultValue.textShadowOffset).value<QPointF>();
        defaultValue.fontFamily = settings.value("FontFamily", defaultValue.fontFamily).toString();
        GlobalData::displayInfoSubFunctions[i.key()] = defaultValue;
        settings.endGroup();
    }
    settings.endGroup();
}

void GlobalData::writeSubFuncSettingsMap(QSettings& settings)
{
    settings.beginGroup("SubFunctions");
    for (auto i = displayInfoSubFunctions.constBegin(); i != displayInfoSubFunctions.constEnd(); i++) {
        auto currentValue = i.value();
        settings.beginGroup(DisplayInfoSubFunctionUtil::toString(i.key()));
        settings.setValue("Display", currentValue.display);
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
