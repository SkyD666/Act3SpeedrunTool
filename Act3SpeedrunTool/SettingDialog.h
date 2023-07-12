#pragma once

#include "DisplayInfoDialog.h"
#include "ui_SettingDialog.h"
#include <QDialog>

class SettingDialog : public QDialog {
    Q_OBJECT

public:
    explicit SettingDialog(QWidget* parent = nullptr, DisplayInfoDialog* displayInfoDialog = nullptr);
    ~SettingDialog();

    QString pageName[4] = { tr("全局热键"), tr("信息展示"), tr("提示音"), tr("语言") };
    const static QString pageIcon[];

protected:
    QString getSoundFile();

    void initDisplayInfoSettings();

private:
    Ui::SettingDialog ui;

    DisplayInfoDialog* displayInfoDialog = nullptr;
};
