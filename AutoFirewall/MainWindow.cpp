#include "MainWindow.h"
#include "FirewallUtil.h"
#include "GlobalData.h"
#include "SettingDialog.h"
#include <MMSystem.h>
#include <QDesktopServices>
#include <QHotkey>
#include <QMessageBox>
#include <QPalette>
#include <QUrl>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , hotkey(nullptr)
    , labCurrentHotkey(new QLabel(this))
    , labState(new QLabel(this))
{
    ui.setupUi(this);

    FirewallUtil::init();

    setHotkey(GlobalData::hotkey);

    labCurrentHotkey->setText(GlobalData::hotkey);
    ui.statusbar->addPermanentWidget(labCurrentHotkey);

    labState->setAutoFillBackground(true);
    labState->setMinimumWidth(16);
    QPalette palette = labState->palette();
    palette.setColor(QPalette::Window, Qt::red);
    labState->setPalette(palette);
    ui.statusbar->addPermanentWidget(labState);

    ui.cbSound->setChecked(GlobalData::playSound);
    connect(ui.cbSound, &QCheckBox::stateChanged, this, [=](int state) {
        GlobalData::playSound = state;
    });

    ui.btnEnable->setText(tr("已关闭"));
    connect(ui.btnEnable, &QAbstractButton::toggled, this, [=](bool checked) {
        if (checked == FirewallUtil::getIsEnabled()) {
            return;
        }
        bool succeed = FirewallUtil::setNetFwRuleEnabled(checked);
        if (succeed) {
            if (checked) {
                ui.btnEnable->setText(tr("已开启"));
                QPalette palette = labState->palette();
                palette.setColor(QPalette::Window, Qt::green);
                labState->setPalette(palette);
                if (GlobalData::playSound) {
                    wchar_t* path = new wchar_t[GlobalData::startSound.size()];
                    GlobalData::startSound.toWCharArray(path);
                    PlaySound(path, nullptr, SND_FILENAME | SND_ASYNC);
                    delete[] path;
                }
            } else {
                ui.btnEnable->setText(tr("已关闭"));
                QPalette palette = labState->palette();
                palette.setColor(QPalette::Window, Qt::red);
                labState->setPalette(palette);
                if (GlobalData::playSound) {
                    wchar_t* path = new wchar_t[GlobalData::stopSound.size()];
                    GlobalData::stopSound.toWCharArray(path);
                    PlaySound(path, nullptr, SND_FILENAME | SND_ASYNC);
                    delete[] path;
                }
            }
        } else {
            ui.btnEnable->setChecked(!checked);
        }
    });

    connect(ui.actionSetting, &QAction::triggered, this, [=]() {
        auto dialog = new SettingDialog(this);
        removeHotkey();
        dialog->exec();
        setHotkey(GlobalData::hotkey);
    });

    connect(ui.actionGitHub, &QAction::triggered, this, [=]() {
        QDesktopServices::openUrl(QUrl("https://github.com/SkyD666/AutoFirewall"));
    });

    connect(ui.actionAboutQt, &QAction::triggered, this, [=]() { qApp->aboutQt(); });

    connect(ui.actionAbout, &QAction::triggered, this, [=]() {
        QMessageBox::about(this, QString(), GlobalData::license);
    });

    ui.btnEnable->setFocus();
}

MainWindow::~MainWindow()
{
    FirewallUtil::release();
}

void MainWindow::removeHotkey()
{
    if (!hotkey) {
        return;
    }
    hotkey->setRegistered(false);
    hotkey->disconnect();
    delete hotkey;
    hotkey = nullptr;
}

void MainWindow::setHotkey(const QString hotkeyStr)
{
    labCurrentHotkey->setText(hotkeyStr);
    if (hotkeyStr.isEmpty()) {
        return;
    }
    hotkey = new QHotkey(QKeySequence(hotkeyStr), true, qApp);
    if (hotkey->isRegistered()) {
        connect(hotkey, &QHotkey::activated, qApp, [=]() {
            ui.btnEnable->toggle();
        });
    } else {
        QMessageBox::critical(nullptr, QString(), tr("热键注册失败！"));
    }
}
