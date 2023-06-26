#pragma once

#include "ui_SettingDialog.h"
#include <QDialog>

class SettingDialog : public QDialog {
    Q_OBJECT

public:
    explicit SettingDialog(QWidget* parent = nullptr);
    ~SettingDialog();

    QString pageName[3] = { tr("全局热键"), tr("提示音"), tr("语言") };
    const static QString pageIcon[];

protected:
    QString getSoundFile();

private:
    Ui::SettingDialog ui;
};
