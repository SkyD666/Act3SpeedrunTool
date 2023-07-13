#include "SettingDialog.h"
#include "GlobalData.h"
#include "LanguageUtil.h"
#include <QColorDialog>
#include <QFileDialog>
#include <QFont>
#include <QFontComboBox>
#include <QPair>
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

    initHotkeySettings();

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

void SettingDialog::initHotkeySettings()
{
    // 防火墙
    ui.keySeqStartFirewall->setKeySequence(QKeySequence(GlobalData::startFirewallHotkey));
    connect(ui.keySeqStartFirewall, &QKeySequenceEdit::editingFinished, this, [=]() {
        if (ui.keySeqStartFirewall->keySequence().count() > 1) {
            QKeyCombination value = ui.keySeqStartFirewall->keySequence()[0];
            QKeySequence shortcut(value);
            ui.keySeqStartFirewall->setKeySequence(shortcut);
            GlobalData::startFirewallHotkey = shortcut.toString();
        } else {
            GlobalData::startFirewallHotkey = ui.keySeqStartFirewall->keySequence().toString();
        }
    });

    connect(ui.tbClearStartFirewallHotkeyEdit, &QAbstractButton::clicked, this, [=]() {
        ui.keySeqStartFirewall->clear();
        GlobalData::startFirewallHotkey = "";
    });

    ui.keySeqStopFirewall->setKeySequence(QKeySequence(GlobalData::stopFirewallHotkey));
    connect(ui.keySeqStopFirewall, &QKeySequenceEdit::editingFinished, this, [=]() {
        if (ui.keySeqStopFirewall->keySequence().count() > 1) {
            QKeyCombination value = ui.keySeqStopFirewall->keySequence()[0];
            QKeySequence shortcut(value);
            ui.keySeqStopFirewall->setKeySequence(shortcut);
            GlobalData::stopFirewallHotkey = shortcut.toString();
        } else {
            GlobalData::stopFirewallHotkey = ui.keySeqStopFirewall->keySequence().toString();
        }
    });

    connect(ui.tbClearStopFirewallHotkeyEdit, &QAbstractButton::clicked, this, [=]() {
        ui.keySeqStopFirewall->clear();
        GlobalData::stopFirewallHotkey = "";
    });

    // 计时器
    ui.keySeqStartTimer->setKeySequence(QKeySequence(GlobalData::startTimerHotkey));
    connect(ui.keySeqStartTimer, &QKeySequenceEdit::editingFinished, this, [=]() {
        if (ui.keySeqStartTimer->keySequence().count() > 1) {
            QKeyCombination value = ui.keySeqStartTimer->keySequence()[0];
            QKeySequence shortcut(value);
            ui.keySeqStartTimer->setKeySequence(shortcut);
            GlobalData::startTimerHotkey = shortcut.toString();
        } else {
            GlobalData::startTimerHotkey = ui.keySeqStartTimer->keySequence().toString();
        }
    });

    connect(ui.tbClearStartTimerHotkeyEdit, &QAbstractButton::clicked, this, [=]() {
        ui.keySeqStartTimer->clear();
        GlobalData::startTimerHotkey = "";
    });

    ui.keySeqPauseTimer->setKeySequence(QKeySequence(GlobalData::pauseTimerHotkey));
    connect(ui.keySeqPauseTimer, &QKeySequenceEdit::editingFinished, this, [=]() {
        if (ui.keySeqPauseTimer->keySequence().count() > 1) {
            QKeyCombination value = ui.keySeqPauseTimer->keySequence()[0];
            QKeySequence shortcut(value);
            ui.keySeqPauseTimer->setKeySequence(shortcut);
            GlobalData::pauseTimerHotkey = shortcut.toString();
        } else {
            GlobalData::pauseTimerHotkey = ui.keySeqPauseTimer->keySequence().toString();
        }
    });

    connect(ui.tbClearPauseTimerHotkeyEdit, &QAbstractButton::clicked, this, [=]() {
        ui.keySeqPauseTimer->clear();
        GlobalData::pauseTimerHotkey = "";
    });

    ui.keySeqStopTimer->setKeySequence(QKeySequence(GlobalData::stopTimerHotkey));
    connect(ui.keySeqStopTimer, &QKeySequenceEdit::editingFinished, this, [=]() {
        if (ui.keySeqStopTimer->keySequence().count() > 1) {
            QKeyCombination value = ui.keySeqStopTimer->keySequence()[0];
            QKeySequence shortcut(value);
            ui.keySeqStopTimer->setKeySequence(shortcut);
            GlobalData::stopTimerHotkey = shortcut.toString();
        } else {
            GlobalData::stopTimerHotkey = ui.keySeqStopTimer->keySequence().toString();
        }
    });

    connect(ui.tbClearStopTimerHotkeyEdit, &QAbstractButton::clicked, this, [=]() {
        ui.keySeqStopTimer->clear();
        GlobalData::stopTimerHotkey = "";
    });
}

void SettingDialog::initDisplayInfoSettings()
{
    RECT rect;
    rect.left = GetSystemMetrics(SM_XVIRTUALSCREEN);
    rect.top = GetSystemMetrics(SM_YVIRTUALSCREEN);
    rect.right = GetSystemMetrics(SM_CXVIRTUALSCREEN) + rect.left;
    rect.bottom = GetSystemMetrics(SM_CYVIRTUALSCREEN) + rect.top;
    // XY
    ui.hsDisplayInfoX->setRange(0, qMax(rect.right - rect.left, GlobalData::displayInfoPos.x()));
    ui.hsDisplayInfoX->setValue(GlobalData::displayInfoPos.x());
    ui.hsDisplayInfoY->setRange(0, qMax(rect.bottom - rect.top, GlobalData::displayInfoPos.y()));
    ui.hsDisplayInfoY->setValue(GlobalData::displayInfoPos.y());
    ui.labDisplayInfoPos->setText(QString("%1, %2")
                                      .arg(GlobalData::displayInfoPos.x())
                                      .arg(GlobalData::displayInfoPos.y()));
    connect(ui.hsDisplayInfoX, &QAbstractSlider::valueChanged, this, [=](int value) {
        GlobalData::displayInfoPos.rx() = value;
        ui.labDisplayInfoPos->setText(QString("%1, %2")
                                          .arg(value)
                                          .arg(GlobalData::displayInfoPos.x()));
        if (displayInfoDialog) {
            displayInfoDialog->move(GlobalData::displayInfoPos);
        }
    });
    connect(ui.hsDisplayInfoY, &QAbstractSlider::valueChanged, this, [=](int value) {
        GlobalData::displayInfoPos.ry() = value;
        ui.labDisplayInfoPos->setText(QString("%1, %2")
                                          .arg(GlobalData::displayInfoPos.y())
                                          .arg(value));
        if (displayInfoDialog) {
            displayInfoDialog->move(GlobalData::displayInfoPos);
        }
    });
    // 宽高
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
            displayInfoDialog->setGeometry(QRect(GlobalData::displayInfoPos, GlobalData::displayInfoSize));
        }
    });
    connect(ui.hsDisplayInfoHeight, &QAbstractSlider::valueChanged, this, [=](int value) {
        GlobalData::displayInfoSize.rheight() = value;
        ui.labDisplayInfoSize->setText(QString("%1 x %2")
                                           .arg(GlobalData::displayInfoSize.width())
                                           .arg(value));
        if (displayInfoDialog) {
            displayInfoDialog->setGeometry(QRect(GlobalData::displayInfoPos, GlobalData::displayInfoSize));
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

    // 内容对齐
    QList<QPair<int, QString>> horiAlign = {
        std::pair(Qt::AlignLeft, tr("左对齐")),
        std::pair(Qt::AlignHCenter, tr("居中")),
        std::pair(Qt::AlignRight, tr("右对齐"))
    };
    for (int i = 0; i < horiAlign.count(); i++) {
        ui.cbDisplayInfoTextHAlign->addItem(horiAlign[i].second, horiAlign[i].first);
        if (horiAlign[i].first == (GlobalData::displayInfoTextAlignment & Qt::AlignHorizontal_Mask).toInt()) {
            ui.cbDisplayInfoTextHAlign->setCurrentIndex(i);
        }
    }
    connect(ui.cbDisplayInfoTextHAlign, QOverload<int>::of(&QComboBox::currentIndexChanged),
        this, [=](int index) {
            GlobalData::displayInfoTextAlignment = Qt::Alignment(
                ui.cbDisplayInfoTextHAlign->itemData(index).toInt()
                | GlobalData::displayInfoTextAlignment & Qt::AlignVertical_Mask);
            displayInfoDialog->setTextAlignment(GlobalData::displayInfoTextAlignment);
        });

    QList<QPair<int, QString>> vertAlign = {
        std::pair(Qt::AlignTop, tr("上对齐")),
        std::pair(Qt::AlignVCenter, tr("居中")),
        std::pair(Qt::AlignBottom, tr("下对齐"))
    };
    for (int i = 0; i < vertAlign.count(); i++) {
        ui.cbDisplayInfoTextVAlign->addItem(vertAlign[i].second, vertAlign[i].first);
        if (vertAlign[i].first == (GlobalData::displayInfoTextAlignment & Qt::AlignVertical_Mask).toInt()) {
            ui.cbDisplayInfoTextVAlign->setCurrentIndex(i);
        }
    }
    connect(ui.cbDisplayInfoTextVAlign, QOverload<int>::of(&QComboBox::currentIndexChanged),
        this, [=](int index) {
            GlobalData::displayInfoTextAlignment = Qt::Alignment(
                ui.cbDisplayInfoTextVAlign->itemData(index).toInt()
                | GlobalData::displayInfoTextAlignment & Qt::AlignHorizontal_Mask);
            displayInfoDialog->setTextAlignment(GlobalData::displayInfoTextAlignment);
        });
}
