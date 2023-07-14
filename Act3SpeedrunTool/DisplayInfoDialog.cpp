#include "DisplayInfoDialog.h"
#include "GlobalData.h"
#include "ui_DisplayInfoDialog.h"

#include <QGraphicsDropShadowEffect>
#include <QMouseEvent>
#include <windows.h>

const QString DisplayInfoDialog::timePattern = "<html><head/><body><p><span style=\" font-size:%1pt;\">%2:%3</span><span style=\" font-size:%4pt;\">.%5</span></p></body></html>";
const QString DisplayInfoDialog::textQssPattern = "color: %1;";
DisplayInfoDialog::DisplayInfoDialog(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::DisplayInfoDialog)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_DeleteOnClose);

    setCursor(Qt::OpenHandCursor);

    setTouchable();
    setChildrenTransparentForMouseEvents();
    setDisplay();
    setTextAlignment();
    setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setGeometry(QRect(GlobalData::displayInfoPos, GlobalData::displayInfoSize));

    setDialogBackground();
    setFont();
    setTextStyle();

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

void DisplayInfoDialog::setDisplay()
{
    ui->labHeadShotCount->setVisible(GlobalData::subFunctionSettings[SubFunction::Headshot].display);
    ui->labTimer->setVisible(GlobalData::subFunctionSettings[SubFunction::Timer].display);
}

void DisplayInfoDialog::setTextAlignment()
{
    ui->labHeadShotCount->setAlignment(GlobalData::subFunctionSettings[SubFunction::Headshot].textAlignment);
    ui->labTimer->setAlignment(GlobalData::subFunctionSettings[SubFunction::Timer].textAlignment);
}

void DisplayInfoDialog::setFont()
{
    ui->labHeadShotCount->setFont(
        QFont(GlobalData::subFunctionSettings[SubFunction::Headshot].fontFamily,
            GlobalData::subFunctionSettings[SubFunction::Headshot].textSize));
    ui->labTimer->setFont(
        QFont(GlobalData::subFunctionSettings[SubFunction::Timer].fontFamily,
            GlobalData::subFunctionSettings[SubFunction::Timer].textSize));
    setTime(0, 0, 0); // 更新字体大小
}

void DisplayInfoDialog::setTextStyle()
{
    ui->labHeadShotCount->setStyleSheet(
        textQssPattern.arg(GlobalData::subFunctionSettings[SubFunction::Headshot].textColor.name()));
    ui->labTimer->setStyleSheet(
        textQssPattern.arg(GlobalData::subFunctionSettings[SubFunction::Timer].textColor.name()));

    QGraphicsDropShadowEffect* headshotEffect = new QGraphicsDropShadowEffect(this);
    headshotEffect->setColor(GlobalData::subFunctionSettings[SubFunction::Headshot].textShadowColor);
    headshotEffect->setBlurRadius(GlobalData::subFunctionSettings[SubFunction::Headshot].textShadowBlurRadius);
    headshotEffect->setOffset(GlobalData::subFunctionSettings[SubFunction::Headshot].textShadowOffset);
    ui->labHeadShotCount->setGraphicsEffect(headshotEffect);

    QGraphicsDropShadowEffect* timerEffect = new QGraphicsDropShadowEffect(this);
    timerEffect->setColor(GlobalData::subFunctionSettings[SubFunction::Timer].textShadowColor);
    timerEffect->setBlurRadius(GlobalData::subFunctionSettings[SubFunction::Timer].textShadowBlurRadius);
    timerEffect->setOffset(GlobalData::subFunctionSettings[SubFunction::Timer].textShadowOffset);
    ui->labTimer->setGraphicsEffect(timerEffect);
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
    auto textSize = GlobalData::subFunctionSettings[SubFunction::Timer].textSize;
    ui->labTimer->setText(timePattern
                              .arg(QString::number(textSize))
                              .arg(m, 2, 10, QLatin1Char('0'))
                              .arg(s, 2, 10, QLatin1Char('0'))
                              .arg(QString::number(qMax(5, textSize - 10)))
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
