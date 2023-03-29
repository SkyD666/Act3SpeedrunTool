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
};
