#pragma once

#include "ui_MainWindow.h"
#include <QHotkey>
#include <QLabel>
#include <QMainWindow>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

protected:
    void removeHotkey();

    void setHotkey(const QString hotkeyStr);

private:
    Ui::MainWindow ui;

    QHotkey* hotkey;

    QLabel* labCurrentHotkey;

    QLabel* labState;

    QString license = "<h3>" + QApplication::applicationDisplayName()
        + " v" + QApplication::applicationVersion() + "</h3>"
        + tr("<p>一个快速启用/停用防火墙规则的工具</p>")
        + "<p>" + tr("作者：") + QApplication::organizationName() + "</p>"
        + "<a href='https://discord.gg/pEWEjeJTa3'>" + tr("加入 Discord 一起划水~") + "</a>"
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
};
