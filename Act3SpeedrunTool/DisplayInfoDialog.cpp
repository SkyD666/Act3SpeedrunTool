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
    ui->labHeadShotCount->setVisible(GlobalData::displayInfoSubFunctions[DisplayInfoSubFunction::Headshot].display);
    ui->labTimer->setVisible(GlobalData::displayInfoSubFunctions[DisplayInfoSubFunction::Timer].display);
}

void DisplayInfoDialog::setTextAlignment()
{
    ui->labHeadShotCount->setAlignment(GlobalData::displayInfoSubFunctions[DisplayInfoSubFunction::Headshot].textAlignment);
    ui->labTimer->setAlignment(GlobalData::displayInfoSubFunctions[DisplayInfoSubFunction::Timer].textAlignment);
}

void DisplayInfoDialog::setFont()
{
    ui->labHeadShotCount->setFont(
        QFont(GlobalData::displayInfoSubFunctions[DisplayInfoSubFunction::Headshot].fontFamily,
            GlobalData::displayInfoSubFunctions[DisplayInfoSubFunction::Headshot].textSize));
    ui->labTimer->setFont(
        QFont(GlobalData::displayInfoSubFunctions[DisplayInfoSubFunction::Timer].fontFamily,
            GlobalData::displayInfoSubFunctions[DisplayInfoSubFunction::Timer].textSize));
    setTime(0, 0, 0); // 更新字体大小
}

void DisplayInfoDialog::setTextStyle()
{
    ui->labHeadShotCount->setStyleSheet(
        textQssPattern.arg(GlobalData::displayInfoSubFunctions[DisplayInfoSubFunction::Headshot].textColor.name()));
    ui->labTimer->setStyleSheet(
        textQssPattern.arg(GlobalData::displayInfoSubFunctions[DisplayInfoSubFunction::Timer].textColor.name()));

    QGraphicsDropShadowEffect* headshotEffect = new QGraphicsDropShadowEffect(this);
    headshotEffect->setColor(GlobalData::displayInfoSubFunctions[DisplayInfoSubFunction::Headshot].textShadowColor);
    headshotEffect->setBlurRadius(GlobalData::displayInfoSubFunctions[DisplayInfoSubFunction::Headshot].textShadowBlurRadius);
    headshotEffect->setOffset(GlobalData::displayInfoSubFunctions[DisplayInfoSubFunction::Headshot].textShadowOffset);
    ui->labHeadShotCount->setGraphicsEffect(headshotEffect);

    QGraphicsDropShadowEffect* timerEffect = new QGraphicsDropShadowEffect(this);
    timerEffect->setColor(GlobalData::displayInfoSubFunctions[DisplayInfoSubFunction::Timer].textShadowColor);
    timerEffect->setBlurRadius(GlobalData::displayInfoSubFunctions[DisplayInfoSubFunction::Timer].textShadowBlurRadius);
    timerEffect->setOffset(GlobalData::displayInfoSubFunctions[DisplayInfoSubFunction::Timer].textShadowOffset);
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
    auto textSize = GlobalData::displayInfoSubFunctions[DisplayInfoSubFunction::Timer].textSize;
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
