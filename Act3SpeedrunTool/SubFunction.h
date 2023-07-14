#include <QColor>
#include <QFont>
#include <QObject>
#include <QPointF>

#pragma once

enum SubFunction {
    Firewall = 1,
    Headshot = 2,
    Timer = 3
};

namespace SubFunctionUtil {
QString toDisplayString(SubFunction f);
QString toString(SubFunction f);
}

struct SubFunctionSettingItem {
    SubFunctionSettingItem()
    {
        display = true;
        textAlignment = Qt::AlignLeft | Qt::AlignVCenter;
        textSize = 26;
        textColor = Qt::yellow;
        textShadowColor = Qt::black;
        textShadowBlurRadius = 7;
        textShadowOffset = { 0, 0 };
        fontFamily = QFont().family();
    };
    SubFunctionSettingItem(bool display, Qt::Alignment textAlignment, int textSize,
        QColor textColor, QColor textShadowColor, int textShadowBlurRadius,
        QPointF textShadowOffset, QString fontFamily)
    {
        this->display = display;
        this->textAlignment = textAlignment;
        this->textAlignment = textAlignment;
        this->textSize = textSize;
        this->textColor = textColor;
        this->textShadowColor = textShadowColor;
        this->textShadowBlurRadius = textShadowBlurRadius;
        this->textShadowOffset = textShadowOffset;
        this->fontFamily = fontFamily;
    }
    bool display;
    Qt::Alignment textAlignment;
    int textSize;
    QColor textColor;
    QColor textShadowColor;
    int textShadowBlurRadius;
    QPointF textShadowOffset;
    QString fontFamily;
};
