#include "SettingDialog.h"
#include "GlobalData.h"
#include "LanguageUtil.h"
#include <QFileDialog>

const QString SettingDialog::pageIcon[] = { ":/ic_settings.svg", ":/ic_search.svg", ":/ic_key_f.svg" };

SettingDialog::SettingDialog(QWidget* parent)
    : QDialog(parent)
{
    ui.setupUi(this);

    setAttribute(Qt::WA_DeleteOnClose);

    {
        int i = 0;
        for (auto name : pageName) {
            QListWidgetItem* item = new QListWidgetItem(name, ui.lwPage);
            //            item->setIcon(QIcon(pageIcon[i]));
            ui.lwPage->addItem(item);
            i++;
        }
    }
    connect(ui.lwPage, &QListWidget::currentRowChanged, ui.stackedWidget, &QStackedWidget::setCurrentIndex);

    ui.keySeqStart->setKeySequence(QKeySequence(GlobalData::hotkey));
    connect(ui.keySeqStart, &QKeySequenceEdit::editingFinished, this, [=]() {
        if (ui.keySeqStart->keySequence().count() > 1) {
            QKeyCombination value = ui.keySeqStart->keySequence()[0];
            QKeySequence shortcut(value);
            ui.keySeqStart->setKeySequence(shortcut);
            GlobalData::hotkey = shortcut.toString();
        } else {
            GlobalData::hotkey = ui.keySeqStart->keySequence().toString();
        }
    });

    connect(ui.tbClearStartHotkeyEdit, &QAbstractButton::clicked, this, [=]() {
        ui.keySeqStart->clear();
        GlobalData::hotkey = "";
    });

    ui.keySeqStop->setKeySequence(QKeySequence(GlobalData::hotkeyStop));
    connect(ui.keySeqStop, &QKeySequenceEdit::editingFinished, this, [=]() {
        if (ui.keySeqStop->keySequence().count() > 1) {
            QKeyCombination value = ui.keySeqStop->keySequence()[0];
            QKeySequence shortcut(value);
            ui.keySeqStop->setKeySequence(shortcut);
            GlobalData::hotkeyStop = shortcut.toString();
        } else {
            GlobalData::hotkeyStop = ui.keySeqStop->keySequence().toString();
        }
    });

    connect(ui.tbClearStopHotkeyEdit, &QAbstractButton::clicked, this, [=]() {
        ui.keySeqStop->clear();
        GlobalData::hotkeyStop = "";
    });

    ui.leStartSoundPath->setText(GlobalData::startSound);
    connect(ui.tbSelectStartSound, &QAbstractButton::clicked, this, [=]() {
        QString fileName = getSoundFile();
        if (!fileName.isEmpty()) {
            ui.leStartSoundPath->setText(fileName);
            GlobalData::startSound = fileName;
        }
    });

    ui.leStopSoundPath->setText(GlobalData::stopSound);
    connect(ui.tbSelectStopSound, &QAbstractButton::clicked, this, [=]() {
        QString fileName = getSoundFile();
        if (!fileName.isEmpty()) {
            ui.leStopSoundPath->setText(fileName);
            GlobalData::stopSound = fileName;
        }
    });

    ui.leErrorSoundPath->setText(GlobalData::errorSound);
    connect(ui.tbSelectErrorSound, &QAbstractButton::clicked, this, [=]() {
        QString fileName = getSoundFile();
        if (!fileName.isEmpty()) {
            ui.leErrorSoundPath->setText(fileName);
            GlobalData::errorSound = fileName;
        }
    });

    for (auto l : LanguageUtil::getInstance()->languages) {
        ui.cbLanguage->addItem(LanguageUtil::getDisplayName(l.name), l.name);
        if (l.name == GlobalData::language) {
            ui.cbLanguage->setCurrentIndex(ui.cbLanguage->count() - 1);
        }
    }
    connect(ui.cbLanguage, &QComboBox::activated, this, [=]() {
        GlobalData::language = ui.cbLanguage->currentData().toString();
        LanguageUtil::applyLanguage();
    });
}

SettingDialog::~SettingDialog()
{
}

QString SettingDialog::getSoundFile()
{
    return QFileDialog::getOpenFileName(this, tr("选择文件"),
        QString(), tr("WAV 文件 (*.wav)"));
}
