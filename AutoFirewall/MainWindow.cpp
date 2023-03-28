#include "MainWindow.h"
#include "FirewallUtil.h"
#include "GlobalData.h"
#include <MMSystem.h>
#include <QDesktopServices>
#include <QHotkey>
#include <QMessageBox>
#include <QUrl>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , hotkey(nullptr)
{
    ui.setupUi(this);

    FirewallUtil::init();

    setHotkey(GlobalData::hotkey);
    ui.keySeq->setKeySequence(QKeySequence(GlobalData::hotkey));

    connect(ui.keySeq, &QKeySequenceEdit::editingFinished, this, [=]() {
        if (ui.keySeq->keySequence().count() > 1) {
            QKeyCombination value = ui.keySeq->keySequence()[0];
            QKeySequence shortcut(value);
            ui.keySeq->setKeySequence(shortcut);
        }
    });

    connect(ui.tbHotkeyEditFinished, &QAbstractButton::clicked, this, [=]() {
        auto newHotkey = ui.keySeq->keySequence().toString();
        if (newHotkey == GlobalData::hotkey) {
            return;
        }
        GlobalData::hotkey = newHotkey;
        removeHotkey();
        setHotkey(GlobalData::hotkey);
        ui.btnEnable->setFocus();
    });

    connect(ui.tbClearHotkeyEdit, &QAbstractButton::clicked, this, [=]() {
        ui.keySeq->clear();
        GlobalData::hotkey = "";
        removeHotkey();
        ui.btnEnable->setFocus();
    });

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
            if (GlobalData::playSound) {
                PlaySound(L"./sound/sound.wav", nullptr, SND_FILENAME | SND_ASYNC);
            }
            if (checked) {
                ui.btnEnable->setText(tr("已开启"));
            } else {
                ui.btnEnable->setText(tr("已关闭"));
            }
        } else {
            ui.btnEnable->setChecked(!checked);
        }
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
