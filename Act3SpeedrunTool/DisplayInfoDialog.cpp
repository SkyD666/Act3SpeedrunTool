#include "DisplayInfoDialog.h"
#include "GlobalData.h"
#include "ui_DisplayInfoDialog.h"

#include <QMouseEvent>
#include <windows.h>

const QString DisplayInfoDialog::timePattern = "<html><head/><body><p><span style=\" font-size:%1pt;\">%2:%3</span><span style=\" font-size:%4pt;\">.%5</span></p></body></html>";

DisplayInfoDialog::DisplayInfoDialog(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::DisplayInfoDialog)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_DeleteOnClose);

    setCursor(Qt::OpenHandCursor);

    setTouchable(GlobalData::displayInfoTouchable);
    setChildrenTransparentForMouseEvents();
    setTextAlignment(GlobalData::displayInfoTextAlignment);
    setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setGeometry(QRect(GlobalData::displayInfoPos, GlobalData::displayInfoSize));

    setDialogBackground(GlobalData::displayInfoBackground);
    setFont(GlobalData::displayInfoFontFamily, GlobalData::displayInfoTextSize);
    setTextColor(GlobalData::displayInfoTextColor);

    setHeadShotCount(0);
    setTime(0, 0, 0);
}

DisplayInfoDialog::~DisplayInfoDialog()
{
    GlobalData::displayInfoSize = { width(), height() };
    delete ui;
}

void DisplayInfoDialog::setDialogBackground(QColor color)
{
    ui->widget->setStyleSheet("#widget { background-color: " + color.name(QColor::HexArgb) + "; }");
}

void DisplayInfoDialog::setTextAlignment(Qt::Alignment alignment)
{
    ui->labHeadShotCount->setAlignment(alignment);
    ui->labTimer->setAlignment(alignment);
}

void DisplayInfoDialog::setFont(QString family, int size)
{
    QFont font = QFont(family, size);
    ui->labHeadShotCount->setFont(font);
    ui->labTimer->setFont(font);
    setTime(0, 0, 0); // 更新字体大小
}

void DisplayInfoDialog::setTextColor(QColor color)
{
    QString c = "color: " + color.name() + ";";
    ui->labHeadShotCount->setStyleSheet(c);
    ui->labTimer->setStyleSheet(c);
}

void DisplayInfoDialog::setTouchable(bool touchable)
{
    setAttribute(Qt::WA_TransparentForMouseEvents, !touchable);
}

void DisplayInfoDialog::setChildrenTransparentForMouseEvents(bool transparent)
{
    ui->widget->setAttribute(Qt::WA_TransparentForMouseEvents, transparent);
    ui->labHeadShotCount->setAttribute(Qt::WA_TransparentForMouseEvents, transparent);
    ui->labTimer->setAttribute(Qt::WA_TransparentForMouseEvents, transparent);
}

void DisplayInfoDialog::setHeadShotCount(short count)
{
    ui->labHeadShotCount->setText(QString::number(count));
}

void DisplayInfoDialog::setTime(int m, int s, int ms)
{
    ui->labTimer->setText(timePattern
                              .arg(QString::number(GlobalData::displayInfoTextSize))
                              .arg(m, 2, 10, QLatin1Char('0'))
                              .arg(s, 2, 10, QLatin1Char('0'))
                              .arg(QString::number(qMax(5, GlobalData::displayInfoTextSize - 10)))
                              .arg(ms, 2, 10, QLatin1Char('0')));
}

void DisplayInfoDialog::mousePressEvent(QMouseEvent* event)
{
    mousePressedPos = event->pos();
    setCursor(Qt::ClosedHandCursor);
    QDialog::mousePressEvent(event);
}

void DisplayInfoDialog::mouseReleaseEvent(QMouseEvent* event)
{
    setCursor(Qt::OpenHandCursor);
    GlobalData::displayInfoPos = { x(), y() };
    QDialog::mouseReleaseEvent(event);
}

void DisplayInfoDialog::mouseMoveEvent(QMouseEvent* event)
{
    move((event->globalPosition() - mousePressedPos).toPoint());
    QDialog::mouseMoveEvent(event);
}
