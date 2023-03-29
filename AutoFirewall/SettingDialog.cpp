#include "SettingDialog.h"
#include "GlobalData.h"
#include <QFileDialog>

const QString SettingDialog::pageName[] = { tr("全局热键"), tr("提示音") };
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

    ui.keySeq->setKeySequence(QKeySequence(GlobalData::hotkey));
    connect(ui.keySeq, &QKeySequenceEdit::editingFinished, this, [=]() {
        if (ui.keySeq->keySequence().count() > 1) {
            QKeyCombination value = ui.keySeq->keySequence()[0];
            QKeySequence shortcut(value);
            ui.keySeq->setKeySequence(shortcut);
            GlobalData::hotkey = shortcut.toString();
        } else {
            GlobalData::hotkey = ui.keySeq->keySequence().toString();
        }
    });

    connect(ui.tbClearHotkeyEdit, &QAbstractButton::clicked, this, [=]() {
        ui.keySeq->clear();
        GlobalData::hotkey = "";
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
}

SettingDialog::~SettingDialog()
{
}

QString SettingDialog::getSoundFile()
{
    return QFileDialog::getOpenFileName(this, tr("选择文件"),
        QString(), tr("WAV 文件 (*.wav)"));
}
