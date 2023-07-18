#pragma once

#include "DisplayInfoDialog.h"
#include "ui_SettingDialog.h"
#include <QDialog>

class SettingDialog : public QDialog {
    Q_OBJECT

public:
    explicit SettingDialog(QWidget* parent = nullptr, DisplayInfoDialog* displayInfoDialog = nullptr);
    ~SettingDialog();

    QString pageName[4] = { tr("信息展示"), tr("断网"), tr("计时器"), tr("语言") };
    const static QString pageIcon[];

protected:
    QString getSoundFile();

    int currentSubFunctionIndex = 0;
    SubFunction currentSubFunction = SubFunction::Headshot;

    void initFirewallSettings();

    void initTimerSettings();

    void initDisplayInfoSettings();

    void setDisplayInfoCententSettings(SubFunction f);

private:
    Ui::SettingDialog ui;

    DisplayInfoDialog* displayInfoDialog = nullptr;

    // 内容对齐
    QList<QPair<int, QString>> horiAlign = {
        std::pair(Qt::AlignLeft, tr("左对齐")),
        std::pair(Qt::AlignHCenter, tr("居中")),
        std::pair(Qt::AlignRight, tr("右对齐"))
    };

    QList<QPair<int, QString>> vertAlign = {
        std::pair(Qt::AlignTop, tr("上对齐")),
        std::pair(Qt::AlignVCenter, tr("居中")),
        std::pair(Qt::AlignBottom, tr("下对齐"))
    };
};
