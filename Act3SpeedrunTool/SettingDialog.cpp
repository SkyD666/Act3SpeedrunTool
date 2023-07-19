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

    initFirewallSettings();
    initHeadshotSettings();
    initTimerSettings();

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

// 防火墙
void SettingDialog::initFirewallSettings()
{
    // 热键
    ui.keySeqStartFirewall->setKeySequence(QKeySequence(GlobalData::firewallStartHotkey));
    connect(ui.keySeqStartFirewall, &QKeySequenceEdit::editingFinished, this, [=]() {
        if (ui.keySeqStartFirewall->keySequence().count() > 1) {
            QKeyCombination value = ui.keySeqStartFirewall->keySequence()[0];
            QKeySequence shortcut(value);
            ui.keySeqStartFirewall->setKeySequence(shortcut);
            GlobalData::firewallStartHotkey = shortcut.toString();
        } else {
            GlobalData::firewallStartHotkey = ui.keySeqStartFirewall->keySequence().toString();
        }
    });

    connect(ui.tbClearStartFirewallHotkeyEdit, &QAbstractButton::clicked, this, [=]() {
        ui.keySeqStartFirewall->clear();
        GlobalData::firewallStartHotkey = "";
    });

    ui.keySeqStopFirewall->setKeySequence(QKeySequence(GlobalData::firewallStopHotkey));
    connect(ui.keySeqStopFirewall, &QKeySequenceEdit::editingFinished, this, [=]() {
        if (ui.keySeqStopFirewall->keySequence().count() > 1) {
            QKeyCombination value = ui.keySeqStopFirewall->keySequence()[0];
            QKeySequence shortcut(value);
            ui.keySeqStopFirewall->setKeySequence(shortcut);
            GlobalData::firewallStopHotkey = shortcut.toString();
        } else {
            GlobalData::firewallStopHotkey = ui.keySeqStopFirewall->keySequence().toString();
        }
    });

    connect(ui.tbClearStopFirewallHotkeyEdit, &QAbstractButton::clicked, this, [=]() {
        ui.keySeqStopFirewall->clear();
        GlobalData::firewallStopHotkey = "";
    });

    // 音效
    ui.leStartSoundPath->setText(GlobalData::firewallStartSound);
    connect(ui.tbSelectStartSound, &QAbstractButton::clicked, this, [=]() {
        QString fileName = getSoundFile();
        if (!fileName.isEmpty()) {
            ui.leStartSoundPath->setText(fileName);
            GlobalData::firewallStartSound = fileName;
        }
    });

    ui.leStopSoundPath->setText(GlobalData::firewallStopSound);
    connect(ui.tbSelectStopSound, &QAbstractButton::clicked, this, [=]() {
        QString fileName = getSoundFile();
        if (!fileName.isEmpty()) {
            ui.leStopSoundPath->setText(fileName);
            GlobalData::firewallStopSound = fileName;
        }
    });

    ui.leErrorSoundPath->setText(GlobalData::firewallErrorSound);
    connect(ui.tbSelectErrorSound, &QAbstractButton::clicked, this, [=]() {
        QString fileName = getSoundFile();
        if (!fileName.isEmpty()) {
            ui.leErrorSoundPath->setText(fileName);
            GlobalData::firewallErrorSound = fileName;
        }
    });

    ui.leFirewallAppPath->setText(GlobalData::firewallAppPath);
    connect(ui.tbSelectFirewallApp, &QAbstractButton::clicked, this, [=]() {
        QString fileName = QFileDialog::getOpenFileName(this, tr("选择程序"),
            GlobalData::firewallAppPath, tr("应用程序 (*.exe)"));
        if (!fileName.isEmpty()) {
            ui.leFirewallAppPath->setText(fileName);
            GlobalData::firewallAppPath = fileName;
        }
    });
    connect(ui.leFirewallAppPath, &QLineEdit::textChanged, this, [=](const QString& text) {
        GlobalData::firewallAppPath = text;
    });
}

// 爆头设置
void SettingDialog::initHeadshotSettings()
{
    // 刷新间隔
    ui.sbHeadshotUpdateInterval->setValue(GlobalData::headshotUpdateInterval);
    connect(ui.sbHeadshotUpdateInterval, &QSpinBox::valueChanged, this, [=](int value) {
        GlobalData::headshotUpdateInterval = value;
        if (displayInfoDialog) {
            displayInfoDialog->setTextStyle();
        }
    });
}

// 计时器设置
void SettingDialog::initTimerSettings()
{
    ui.keySeqStartTimer->setKeySequence(QKeySequence(GlobalData::timerStartHotkey));
    connect(ui.keySeqStartTimer, &QKeySequenceEdit::editingFinished, this, [=]() {
        if (ui.keySeqStartTimer->keySequence().count() > 1) {
            QKeyCombination value = ui.keySeqStartTimer->keySequence()[0];
            QKeySequence shortcut(value);
            ui.keySeqStartTimer->setKeySequence(shortcut);
            GlobalData::timerStartHotkey = shortcut.toString();
        } else {
            GlobalData::timerStartHotkey = ui.keySeqStartTimer->keySequence().toString();
        }
    });

    connect(ui.tbClearStartTimerHotkeyEdit, &QAbstractButton::clicked, this, [=]() {
        ui.keySeqStartTimer->clear();
        GlobalData::timerStartHotkey = "";
    });

    ui.keySeqPauseTimer->setKeySequence(QKeySequence(GlobalData::timerPauseHotkey));
    connect(ui.keySeqPauseTimer, &QKeySequenceEdit::editingFinished, this, [=]() {
        if (ui.keySeqPauseTimer->keySequence().count() > 1) {
            QKeyCombination value = ui.keySeqPauseTimer->keySequence()[0];
            QKeySequence shortcut(value);
            ui.keySeqPauseTimer->setKeySequence(shortcut);
            GlobalData::timerPauseHotkey = shortcut.toString();
        } else {
            GlobalData::timerPauseHotkey = ui.keySeqPauseTimer->keySequence().toString();
        }
    });

    connect(ui.tbClearPauseTimerHotkeyEdit, &QAbstractButton::clicked, this, [=]() {
        ui.keySeqPauseTimer->clear();
        GlobalData::timerPauseHotkey = "";
    });

    ui.keySeqStopTimer->setKeySequence(QKeySequence(GlobalData::timerStopHotkey));
    connect(ui.keySeqStopTimer, &QKeySequenceEdit::editingFinished, this, [=]() {
        if (ui.keySeqStopTimer->keySequence().count() > 1) {
            QKeyCombination value = ui.keySeqStopTimer->keySequence()[0];
            QKeySequence shortcut(value);
            ui.keySeqStopTimer->setKeySequence(shortcut);
            GlobalData::timerStopHotkey = shortcut.toString();
        } else {
            GlobalData::timerStopHotkey = ui.keySeqStopTimer->keySequence().toString();
        }
    });

    connect(ui.tbClearStopTimerHotkeyEdit, &QAbstractButton::clicked, this, [=]() {
        ui.keySeqStopTimer->clear();
        GlobalData::timerStopHotkey = "";
    });

    // 刷新间隔
    ui.sbTimerUpdateInterval->setValue(GlobalData::timerUpdateInterval);
    connect(ui.sbTimerUpdateInterval, &QSpinBox::valueChanged, this, [=](int value) {
        GlobalData::timerUpdateInterval = value;
        if (displayInfoDialog) {
            displayInfoDialog->setTextStyle();
        }
    });

    // 停止计时器策略
    {
        int i = 0;
        int currentTimerStopStrategyIndex = 0;
        for (auto s : GlobalData::timerStopStrategies) {
            ui.cbTimerStopStrategy->addItem(TimerStopStrategyUtil::toDisplayString(s), s);
            if (s == GlobalData::timerStopStrategy) {
                currentTimerStopStrategyIndex = i;
            }
            i++;
        }
        connect(ui.cbTimerStopStrategy, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [=](int index) {
                GlobalData::timerStopStrategy = ui.cbTimerStopStrategy->itemData(index).value<TimerStopStrategy>();
            });
        ui.cbTimerStopStrategy->setCurrentIndex(currentTimerStopStrategyIndex);
    }
}

void SettingDialog::initDisplayInfoSettings()
{
    // 窗体 ============================================================
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
        QColorDialog dialog(GlobalData::displayInfoBackground);
        if (dialog.exec() == QDialog::Accepted) {
            auto c = QColor::fromRgb(dialog.selectedColor().rgb());
            c.setAlpha(GlobalData::displayInfoBackground.alpha());
            GlobalData::displayInfoBackground = c;
            QPalette paletteBackground = ui.labDisplayInfoBackground->palette();
            paletteBackground.setColor(QPalette::Window, c.rgb());
            ui.labDisplayInfoBackground->setPalette(paletteBackground);

            if (displayInfoDialog) {
                displayInfoDialog->setDialogBackground();
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
            displayInfoDialog->setDialogBackground();
        }
    });

    // 内容 ================================================================================
    // 字体family
    connect(ui.fcbDisplayInfoFont, &QFontComboBox::currentFontChanged, this, [=](const QFont& font) {
        GlobalData::displayInfoSubFunctions[currentSubFunction].fontFamily = font.family();
        if (displayInfoDialog) {
            displayInfoDialog->setFont();
        }
    });

    connect(ui.sbDisplayInfoTextSize, &QSpinBox::valueChanged, this, [=](int value) {
        GlobalData::displayInfoSubFunctions[currentSubFunction].textSize = value;
        if (displayInfoDialog) {
            displayInfoDialog->setFont();
        }
    });

    connect(ui.tbSelectDisplayInfoTextColor, &QAbstractButton::clicked, this, [=]() {
        QColorDialog dialog(GlobalData::displayInfoSubFunctions[currentSubFunction].textColor);
        if (dialog.exec() == QDialog::Accepted) {
            auto color = dialog.selectedColor();
            GlobalData::displayInfoSubFunctions[currentSubFunction].textColor = color;
            ui.labDisplayInfoTextColor->setStyleSheet(QString("background-color: %1;").arg(color.name()));

            if (displayInfoDialog) {
                displayInfoDialog->setTextStyle();
            }
        }
    });

    // 阴影
    connect(ui.sbDisplayInfoTextShadowBlurRadius, &QSpinBox::valueChanged, this, [=](int value) {
        GlobalData::displayInfoSubFunctions[currentSubFunction].textShadowBlurRadius = value;
        if (displayInfoDialog) {
            displayInfoDialog->setTextStyle();
        }
    });
    connect(ui.sbDisplayInfoTextShadowOffsetX, &QSpinBox::valueChanged, this, [=](int value) {
        GlobalData::displayInfoSubFunctions[currentSubFunction].textShadowOffset.rx() = value;
        if (displayInfoDialog) {
            displayInfoDialog->setTextStyle();
        }
    });
    connect(ui.sbDisplayInfoTextShadowOffsetY, &QSpinBox::valueChanged, this, [=](int value) {
        GlobalData::displayInfoSubFunctions[currentSubFunction].textShadowOffset.ry() = value;
        if (displayInfoDialog) {
            displayInfoDialog->setTextStyle();
        }
    });
    connect(ui.tbSelectDisplayInfoTextShadowColor, &QAbstractButton::clicked, this, [=]() {
        QColorDialog dialog(GlobalData::displayInfoSubFunctions[currentSubFunction].textShadowColor);
        if (dialog.exec() == QDialog::Accepted) {
            auto color = dialog.selectedColor();
            GlobalData::displayInfoSubFunctions[currentSubFunction].textShadowColor = color;
            ui.labDisplayInfoTextShadowColor->setStyleSheet(
                QString("background-color: %1;").arg(color.name()));

            if (displayInfoDialog) {
                displayInfoDialog->setTextStyle();
            }
        }
    });

    // 内容对齐
    for (int i = 0; i < horiAlign.count(); i++) {
        ui.cbDisplayInfoTextHAlign->addItem(horiAlign[i].second, horiAlign[i].first);
    }
    connect(ui.cbDisplayInfoTextHAlign, QOverload<int>::of(&QComboBox::currentIndexChanged),
        this, [=](int index) {
            auto& currentSetting = GlobalData::displayInfoSubFunctions[currentSubFunction];
            currentSetting.textAlignment = Qt::Alignment(
                ui.cbDisplayInfoTextHAlign->itemData(index).toInt()
                | currentSetting.textAlignment & Qt::AlignVertical_Mask);
            if (displayInfoDialog) {
                displayInfoDialog->setTextAlignment();
            }
        });

    for (int i = 0; i < vertAlign.count(); i++) {
        ui.cbDisplayInfoTextVAlign->addItem(vertAlign[i].second, vertAlign[i].first);
    }
    connect(ui.cbDisplayInfoTextVAlign, QOverload<int>::of(&QComboBox::currentIndexChanged),
        this, [=](int index) {
            auto& currentSetting = GlobalData::displayInfoSubFunctions[currentSubFunction];
            currentSetting.textAlignment = Qt::Alignment(
                ui.cbDisplayInfoTextVAlign->itemData(index).toInt()
                | currentSetting.textAlignment & Qt::AlignHorizontal_Mask);
            if (displayInfoDialog) {
                displayInfoDialog->setTextAlignment();
            }
        });

    // 在窗口展示
    connect(ui.cbDisplayInfoFuncEnable, &QCheckBox::stateChanged, this, [=](int state) {
        GlobalData::displayInfoSubFunctions[currentSubFunction].display = state == Qt::Checked;
        if (displayInfoDialog) {
            displayInfoDialog->setDisplay();
        }
    });

    // 设置分类
    for (auto f : GlobalData::funcs) {
        ui.cbDisplayInfoFunction->addItem(DisplayInfoSubFunctionUtil::toDisplayString(f), f);
    }
    ui.cbDisplayInfoFunction->removeItem(0); // 暂时不显示防火墙
    connect(ui.cbDisplayInfoFunction, QOverload<int>::of(&QComboBox::currentIndexChanged),
        this, [=](int index) {
            currentSubFunctionIndex = index;
            currentSubFunction = ui.cbDisplayInfoFunction->itemData(index).value<DisplayInfoSubFunction>();
            setDisplayInfoCententSettings(currentSubFunction);
        });
    setDisplayInfoCententSettings(currentSubFunction);
    ui.cbDisplayInfoFunction->setCurrentIndex(currentSubFunctionIndex);
}

void SettingDialog::setDisplayInfoCententSettings(DisplayInfoSubFunction f)
{
    auto& currentSetting = GlobalData::displayInfoSubFunctions[f];
    ui.cbDisplayInfoFuncEnable->setChecked(currentSetting.display);
    ui.fcbDisplayInfoFont->setCurrentFont(QFont(currentSetting.fontFamily));
    ui.sbDisplayInfoTextSize->setValue(currentSetting.textSize);
    ui.labDisplayInfoTextColor->setStyleSheet(
        QString("background-color: %1;").arg(currentSetting.textColor.name()));
    ui.sbDisplayInfoTextShadowBlurRadius->setValue(currentSetting.textShadowBlurRadius);
    ui.sbDisplayInfoTextShadowOffsetX->setValue(currentSetting.textShadowOffset.x());
    ui.sbDisplayInfoTextShadowOffsetY->setValue(currentSetting.textShadowOffset.y());
    ui.labDisplayInfoTextShadowColor->setStyleSheet(
        QString("background-color: %1;").arg(currentSetting.textShadowColor.name()));
    for (int i = 0; i < horiAlign.count(); i++) {
        if (horiAlign[i].first == (currentSetting.textAlignment & Qt::AlignHorizontal_Mask).toInt()) {
            ui.cbDisplayInfoTextHAlign->setCurrentIndex(i);
        }
    }
    for (int i = 0; i < vertAlign.count(); i++) {
        if (vertAlign[i].first == (currentSetting.textAlignment & Qt::AlignVertical_Mask).toInt()) {
            ui.cbDisplayInfoTextVAlign->setCurrentIndex(i);
        }
    }
}
