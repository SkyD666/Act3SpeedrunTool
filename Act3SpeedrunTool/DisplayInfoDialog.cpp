#include "DisplayInfoDialog.h"
#include "GlobalData.h"
#include "ui_DisplayInfoDialog.h"

#include <QMouseEvent>

DisplayInfoDialog::DisplayInfoDialog(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::DisplayInfoDialog)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_DeleteOnClose);

    setCursor(Qt::OpenHandCursor);

    setTouchable(GlobalData::displayInfoTouchable);
    setWindowFlag(Qt::WindowStaysOnTopHint);
    setWindowFlag(Qt::FramelessWindowHint);

    setDialogBackground(GlobalData::displayInfoBackground);
    setFixedSize(GlobalData::displayInfoSize);
    setFont(GlobalData::displayInfoFontFamily, GlobalData::displayInfoTextSize);
    setTextColor(GlobalData::displayInfoTextColor);

    move(GlobalData::displayInfoPos);

    ui->labHeadShotCount->setText("0");
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

void DisplayInfoDialog::setFont(QString family, int size)
{
    ui->labHeadShotCount->setFont(QFont(family, size));
}

void DisplayInfoDialog::setTextColor(QColor color)
{
    ui->labHeadShotCount->setStyleSheet("color: " + color.name() + ";");
}

void DisplayInfoDialog::setTouchable(bool touchable)
{
    setAttribute(Qt::WA_TransparentForMouseEvents, !touchable);
    ui->labHeadShotCount->setAttribute(Qt::WA_TransparentForMouseEvents, !touchable);
}

void DisplayInfoDialog::setHeadShotCount(short count)
{
    ui->labHeadShotCount->setText(QString::number(count));
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
