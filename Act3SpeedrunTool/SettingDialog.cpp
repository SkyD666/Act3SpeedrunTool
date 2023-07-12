#include "SettingDialog.h"
#include "GlobalData.h"
#include "LanguageUtil.h"
#include <QColorDialog>
#include <QFileDialog>
#include <QFont>
#include <QFontComboBox>
#include <QScreen>
#include <windows.h>

const QString SettingDialog::pageIcon[] = { ":/ic_settings.svg", ":/ic_search.svg", ":/ic_key_f.svg" };

SettingDialog::SettingDialog(QWidget* parent, DisplayInfoDialog* displayInfoDialog)
    : QDialog(parent)
    , displayInfoDialog(displayInfoDialog)
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

    ui.keySeqStop->setKeySequence(QKeySequence(GlobalData::stopHotkey));
    connect(ui.keySeqStop, &QKeySequenceEdit::editingFinished, this, [=]() {
        if (ui.keySeqStop->keySequence().count() > 1) {
            QKeyCombination value = ui.keySeqStop->keySequence()[0];
            QKeySequence shortcut(value);
            ui.keySeqStop->setKeySequence(shortcut);
            GlobalData::stopHotkey = shortcut.toString();
        } else {
            GlobalData::stopHotkey = ui.keySeqStop->keySequence().toString();
        }
    });

    connect(ui.tbClearStopHotkeyEdit, &QAbstractButton::clicked, this, [=]() {
        ui.keySeqStop->clear();
        GlobalData::stopHotkey = "";
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

    initDisplayInfoSettings();
}

SettingDialog::~SettingDialog()
{
}

QString SettingDialog::getSoundFile()
{
    return QFileDialog::getOpenFileName(this, tr("选择文件"),
        QString(), tr("WAV 文件 (*.wav)"));
}

void SettingDialog::initDisplayInfoSettings()
{
    RECT rect;
    rect.left = GetSystemMetrics(SM_XVIRTUALSCREEN);
    rect.top = GetSystemMetrics(SM_YVIRTUALSCREEN);
    rect.right = GetSystemMetrics(SM_CXVIRTUALSCREEN) + rect.left;
    rect.bottom = GetSystemMetrics(SM_CYVIRTUALSCREEN) + rect.top;
    ui.hsDisplayInfoWidth->setRange(10, qMax(rect.right - rect.left, GlobalData::displayInfoSize.width()));
    ui.hsDisplayInfoWidth->setValue(GlobalData::displayInfoSize.width());
    ui.hsDisplayInfoHeight->setRange(10, qMax(rect.bottom - rect.top, GlobalData::displayInfoSize.height()));
    ui.hsDisplayInfoHeight->setValue(GlobalData::displayInfoSize.height());
    ui.labDisplayInfoSize->setText(QString("%1 x %2")
                                       .arg(GlobalData::displayInfoSize.width())
                                       .arg(GlobalData::displayInfoSize.height()));
    connect(ui.hsDisplayInfoWidth, &QAbstractSlider::valueChanged, this, [=](int value) {
        GlobalData::displayInfoSize.rwidth() = value;
        ui.labDisplayInfoSize->setText(QString("%1 x %2")
                                           .arg(value)
                                           .arg(GlobalData::displayInfoSize.height()));
        if (displayInfoDialog) {
            displayInfoDialog->setFixedWidth(value);
        }
    });
    connect(ui.hsDisplayInfoHeight, &QAbstractSlider::valueChanged, this, [=](int value) {
        GlobalData::displayInfoSize.rheight() = value;
        ui.labDisplayInfoSize->setText(QString("%1 x %2")
                                           .arg(GlobalData::displayInfoSize.width())
                                           .arg(value));
        if (displayInfoDialog) {
            displayInfoDialog->setFixedHeight(value);
        }
    });

    // 窗体背景颜色
    ui.labDisplayInfoBackground->setAutoFillBackground(true);
    QPalette paletteBackground = ui.labDisplayInfoBackground->palette();
    paletteBackground.setColor(QPalette::Window, GlobalData::displayInfoBackground.rgb());
    ui.labDisplayInfoBackground->setPalette(paletteBackground);
    connect(ui.tbSelectDisplayInfoBackground, &QAbstractButton::clicked, this, [=]() {
        QColorDialog dialog(GlobalData::displayInfoTextColor);
        if (dialog.exec() == QDialog::Accepted) {
            auto c = QColor::fromRgb(dialog.selectedColor().rgb());
            c.setAlpha(GlobalData::displayInfoBackground.alpha());
            GlobalData::displayInfoBackground = c;
            QPalette paletteBackground = ui.labDisplayInfoBackground->palette();
            paletteBackground.setColor(QPalette::Window, c.rgb());
            ui.labDisplayInfoBackground->setPalette(paletteBackground);

            if (displayInfoDialog) {
                displayInfoDialog->setDialogBackground(GlobalData::displayInfoBackground);
            }
        }
    });
    ui.hsDisplayInfoAlpha->setRange(0, 255);
    ui.hsDisplayInfoAlpha->setValue(GlobalData::displayInfoBackground.alpha());
    ui.labDisplayInfoAlpha->setText(QString::number(GlobalData::displayInfoBackground.alpha()));
    connect(ui.hsDisplayInfoAlpha, &QAbstractSlider::valueChanged, this, [=](int value) {
        GlobalData::displayInfoBackground.setAlpha(value);
        ui.labDisplayInfoAlpha->setText(QString::number(value));
        if (displayInfoDialog) {
            displayInfoDialog->setDialogBackground(GlobalData::displayInfoBackground);
        }
    });

    // 字体family
    ui.fcbDisplayInfoFont->setCurrentFont(QFont(GlobalData::displayInfoFontFamily));
    connect(ui.fcbDisplayInfoFont, &QFontComboBox::currentFontChanged, this, [=](const QFont& font) {
        GlobalData::displayInfoFontFamily = font.family();
        if (displayInfoDialog) {
            displayInfoDialog->setFont(GlobalData::displayInfoFontFamily, GlobalData::displayInfoTextSize);
        }
    });

    ui.sbDisplayInfoTextSize->setValue(GlobalData::displayInfoTextSize);
    connect(ui.sbDisplayInfoTextSize, &QSpinBox::valueChanged, this, [=](int value) {
        GlobalData::displayInfoTextSize = value;
        if (displayInfoDialog) {
            displayInfoDialog->setFont(GlobalData::displayInfoFontFamily, GlobalData::displayInfoTextSize);
        }
    });

    ui.labDisplayInfoTextColor->setAutoFillBackground(true);
    QPalette palette = ui.labDisplayInfoTextColor->palette();
    palette.setColor(QPalette::Window, GlobalData::displayInfoTextColor);
    ui.labDisplayInfoTextColor->setPalette(palette);
    connect(ui.tbSelectDisplayInfoTextColor, &QAbstractButton::clicked, this, [=]() {
        QColorDialog dialog(GlobalData::displayInfoTextColor);
        if (dialog.exec() == QDialog::Accepted) {
            GlobalData::displayInfoTextColor = dialog.selectedColor();
            QPalette palette = ui.labDisplayInfoTextColor->palette();
            palette.setColor(QPalette::Window, GlobalData::displayInfoTextColor);
            ui.labDisplayInfoTextColor->setPalette(palette);

            if (displayInfoDialog) {
                displayInfoDialog->setTextColor(GlobalData::displayInfoTextColor);
            }
        }
    });
}
