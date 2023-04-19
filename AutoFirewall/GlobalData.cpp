#include "GlobalData.h"
#include <QApplication>
#include <QSettings>

QString GlobalData::hotkey = "F9";
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
    license = license.arg(QApplication::applicationDisplayName(), QApplication::applicationVersion());
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
    settings.endGroup();

    settings.beginGroup("Sound");
    settings.setValue("StartSound", startSound);
    settings.setValue("StopSound", stopSound);
    settings.setValue("ErrorSound", errorSound);
    settings.setValue("PlaySound", playSound);
    settings.endGroup();
}

QString GlobalData::license = "<h3>%1 v%2</h3>" + QObject::tr("<p>一个快速启用/停用防火墙规则的工具</p>")
    + QString("<a href='https://discord.gg/pEWEjeJTa3'>加入 Discord 一起划水~</a>")
    + QString("<p>Copyright<font>&#169;</font> 2023 SkyD666</p>"
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
