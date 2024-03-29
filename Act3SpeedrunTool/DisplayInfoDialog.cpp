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
    qreal devicePixelRatio = screen()->devicePixelRatio();
    setGeometry(QRect(globalData->displayInfoPos() / devicePixelRatio, globalData->displayInfoSize() / devicePixelRatio));

    setDialogBackground();
    setFont();
    setTextStyle();

    setHeadShotCount(0);
    setTime(0, 0, 0);

    initGlobalDataConnects();
}

DisplayInfoDialog::~DisplayInfoDialog()
{
    globalData->setDisplayInfoSize({ width(), height() });
    delete ui;
}

void DisplayInfoDialog::setDialogBackground(QColor color)
{
    ui->widget->setStyleSheet("#widget { background-color: " + color.name(QColor::HexArgb) + "; }");
}

void DisplayInfoDialog::setDisplay()
{
    auto displayInfoSubFunctions = globalData->displayInfoSubFunctions();
    ui->labHeadShotCount->setVisible(displayInfoSubFunctions[DisplayInfoSubFunction::Headshot]->display());
    ui->labTimer->setVisible(displayInfoSubFunctions[DisplayInfoSubFunction::Timer]->display());
}

void DisplayInfoDialog::setTextAlignment()
{
    auto displayInfoSubFunctions = globalData->displayInfoSubFunctions();
    ui->labHeadShotCount->setAlignment(displayInfoSubFunctions[DisplayInfoSubFunction::Headshot]->textAlignment());
    ui->labTimer->setAlignment(displayInfoSubFunctions[DisplayInfoSubFunction::Timer]->textAlignment());
}

void DisplayInfoDialog::setFont()
{
    auto displayInfoSubFunctions = globalData->displayInfoSubFunctions();
    ui->labHeadShotCount->setFont(
        QFont(displayInfoSubFunctions[DisplayInfoSubFunction::Headshot]->fontFamily(),
            displayInfoSubFunctions[DisplayInfoSubFunction::Headshot]->textSize()));
    ui->labTimer->setFont(
        QFont(displayInfoSubFunctions[DisplayInfoSubFunction::Timer]->fontFamily(),
            displayInfoSubFunctions[DisplayInfoSubFunction::Timer]->textSize()));
    setTime(0, 0, 0); // 更新字体大小
}

void DisplayInfoDialog::setTextStyle()
{
    auto displayInfoSubFunctions = globalData->displayInfoSubFunctions();
    auto headshotItem = displayInfoSubFunctions[DisplayInfoSubFunction::Headshot];
    auto timerItem = displayInfoSubFunctions[DisplayInfoSubFunction::Timer];

    setHeadShotTextStyle(
        headshotItem->textColor(),
        headshotItem->textShadowColor(),
        headshotItem->textShadowBlurRadius(),
        headshotItem->textShadowOffset());
    setTimerTextStyle(
        timerItem->textColor(),
        timerItem->textShadowColor(),
        timerItem->textShadowBlurRadius(),
        timerItem->textShadowOffset());
}

void DisplayInfoDialog::setHeadShotTextStyle(
    const QColor& textColor,
    const QColor& textShadowColor,
    qreal textShadowBlurRadius,
    const QPointF& textShadowOffset)
{
    ui->labHeadShotCount->setStyleSheet(
        textQssPattern.arg(textColor.name()));

    QGraphicsDropShadowEffect* headshotEffect = new QGraphicsDropShadowEffect(this);
    headshotEffect->setColor(textShadowColor);
    headshotEffect->setBlurRadius(textShadowBlurRadius);
    headshotEffect->setOffset(textShadowOffset);
    ui->labHeadShotCount->setGraphicsEffect(headshotEffect);
}

void DisplayInfoDialog::setTimerTextStyle(
    const QColor& textColor,
    const QColor& textShadowColor,
    qreal textShadowBlurRadius,
    const QPointF& textShadowOffset)
{
    ui->labTimer->setStyleSheet(
        textQssPattern.arg(textColor.name()));

    QGraphicsDropShadowEffect* timerEffect = new QGraphicsDropShadowEffect(this);
    timerEffect->setColor(textShadowColor);
    timerEffect->setBlurRadius(textShadowBlurRadius);
    timerEffect->setOffset(textShadowOffset);
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
    auto textSize = globalData->displayInfoSubFunctions()[DisplayInfoSubFunction::Timer]->textSize();
    ui->labTimer->setText(timePattern
                              .arg(QString::number(textSize))
                              .arg(m, 2, 10, QLatin1Char('0'))
                              .arg(s, 2, 10, QLatin1Char('0'))
                              .arg(QString::number(qMax(5, textSize - 10)))
                              .arg(ms, 2, 10, QLatin1Char('0')));
}

void DisplayInfoDialog::initGlobalDataConnects()
{
    auto displayInfoSubFunctions = globalData->displayInfoSubFunctions();
    auto headshotItem = displayInfoSubFunctions[DisplayInfoSubFunction::Headshot];
    auto timerItem = displayInfoSubFunctions[DisplayInfoSubFunction::Timer];

    connect(headshotItem, &DisplayInfoSubFunctionItem::displayChanged, this,
        [this](bool newDisplay) {
            ui->labHeadShotCount->setVisible(newDisplay);
        });
    connect(timerItem, &DisplayInfoSubFunctionItem::displayChanged, this,
        [this](bool newDisplay) {
            ui->labTimer->setVisible(newDisplay);
        });

    connect(headshotItem, &DisplayInfoSubFunctionItem::textAlignmentChanged, this,
        [this](const Qt::Alignment& newTextAlignment) {
            ui->labHeadShotCount->setAlignment(newTextAlignment);
        });
    connect(timerItem, &DisplayInfoSubFunctionItem::textAlignmentChanged, this,
        [this](const Qt::Alignment& newTextAlignment) {
            ui->labTimer->setAlignment(newTextAlignment);
        });

    connect(headshotItem, &DisplayInfoSubFunctionItem::textSizeChanged, this,
        [this, headshotItem](int newTextSize) {
            ui->labHeadShotCount->setFont(QFont(headshotItem->fontFamily(), newTextSize));
        });
    connect(timerItem, &DisplayInfoSubFunctionItem::textSizeChanged, this,
        [this, timerItem](int newTextSize) {
            ui->labTimer->setFont(QFont(timerItem->fontFamily(), newTextSize));
        });

    connect(headshotItem, &DisplayInfoSubFunctionItem::textColorChanged, this,
        [this, headshotItem](const QColor& newTextColor) {
            setHeadShotTextStyle(
                newTextColor,
                headshotItem->textShadowColor(),
                headshotItem->textShadowBlurRadius(),
                headshotItem->textShadowOffset());
        });
    connect(timerItem, &DisplayInfoSubFunctionItem::textColorChanged, this,
        [this, timerItem](const QColor& newTextColor) {
            setTimerTextStyle(
                newTextColor,
                timerItem->textShadowColor(),
                timerItem->textShadowBlurRadius(),
                timerItem->textShadowOffset());
        });

    connect(headshotItem, &DisplayInfoSubFunctionItem::textShadowColorChanged, this,
        [this, headshotItem](const QColor& newTextShadowColor) {
            setHeadShotTextStyle(
                headshotItem->textColor(),
                newTextShadowColor,
                headshotItem->textShadowBlurRadius(),
                headshotItem->textShadowOffset());
        });
    connect(timerItem, &DisplayInfoSubFunctionItem::textShadowColorChanged, this,
        [this, timerItem](const QColor& newTextShadowColor) {
            setTimerTextStyle(
                timerItem->textColor(),
                newTextShadowColor,
                timerItem->textShadowBlurRadius(),
                timerItem->textShadowOffset());
        });

    connect(headshotItem, &DisplayInfoSubFunctionItem::textShadowBlurRadiusChanged, this,
        [this, headshotItem](int newTextShadowBlurRadius) {
            setHeadShotTextStyle(
                headshotItem->textColor(),
                headshotItem->textShadowColor(),
                newTextShadowBlurRadius,
                headshotItem->textShadowOffset());
        });
    connect(timerItem, &DisplayInfoSubFunctionItem::textShadowBlurRadiusChanged, this,
        [this, timerItem](int newTextShadowBlurRadius) {
            setTimerTextStyle(
                timerItem->textColor(),
                timerItem->textShadowColor(),
                newTextShadowBlurRadius,
                timerItem->textShadowOffset());
        });

    connect(headshotItem, &DisplayInfoSubFunctionItem::textShadowOffsetChanged, this,
        [this, headshotItem](QPointF newTextShadowOffset) {
            setHeadShotTextStyle(
                headshotItem->textColor(),
                headshotItem->textShadowColor(),
                headshotItem->textShadowBlurRadius(),
                newTextShadowOffset);
        });
    connect(timerItem, &DisplayInfoSubFunctionItem::textShadowOffsetChanged, this,
        [this, timerItem](QPointF newTextShadowOffset) {
            setTimerTextStyle(
                timerItem->textColor(),
                timerItem->textShadowColor(),
                timerItem->textShadowBlurRadius(),
                newTextShadowOffset);
        });

    connect(headshotItem, &DisplayInfoSubFunctionItem::fontFamilyChanged, this,
        [this, headshotItem](const QString& newFontFamily) {
            ui->labHeadShotCount->setFont(QFont(newFontFamily, headshotItem->textSize()));
        });
    connect(timerItem, &DisplayInfoSubFunctionItem::fontFamilyChanged, this,
        [this, timerItem](const QString& newFontFamily) {
            ui->labTimer->setFont(QFont(newFontFamily, timerItem->textSize()));
        });

    // ==========================
    connect(globalData, &GlobalData::displayInfoPosChanged, this, [this]() {
        move(globalData->displayInfoPos() / screen()->devicePixelRatio());
    });
    connect(globalData, &GlobalData::displayInfoSizeChanged, this, [this]() {
        qreal devicePixelRatio = screen()->devicePixelRatio();
        setGeometry(QRect(
            globalData->displayInfoPos() / devicePixelRatio,
            globalData->displayInfoSize() / devicePixelRatio));
    });
    connect(globalData, &GlobalData::displayInfoBackgroundChanged, this, [this]() {
        setDialogBackground();
    });
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
    qreal devicePixelRatio = screen()->devicePixelRatio();
    globalData->setDisplayInfoPos({ static_cast<int>(x() * devicePixelRatio),
        static_cast<int>(y() * devicePixelRatio) });
    QDialog::mouseReleaseEvent(event);
}

void DisplayInfoDialog::mouseMoveEvent(QMouseEvent* event)
{
    move((event->globalPosition() - mousePressedPos).toPoint());
    QDialog::mouseMoveEvent(event);
}
