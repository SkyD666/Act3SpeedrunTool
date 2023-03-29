#pragma once

#include "ui_SettingDialog.h"
#include <QDialog>

class SettingDialog : public QDialog {
    Q_OBJECT

public:
    explicit SettingDialog(QWidget* parent = nullptr);
    ~SettingDialog();

    const static QString pageName[];
    const static QString pageIcon[];

protected:
    QString getSoundFile();

private:
    Ui::SettingDialog ui;
};
