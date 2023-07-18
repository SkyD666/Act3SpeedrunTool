#include "DisplayInfoSubFunction.h"
#include <QColor>
#include <QFont>
#include <QObject>
#include <QPointF>

namespace DisplayInfoSubFunctionUtil {

QString toDisplayString(DisplayInfoSubFunction f)
{
    switch (f) {
    case Firewall:
        return QObject::tr("断网");
        break;
    case Headshot:
        return QObject::tr("爆头");
        break;
    case Timer:
        return QObject::tr("计时器");
        break;
    default:
        return "";
    }
};

QString toString(DisplayInfoSubFunction f)
{
    switch (f) {
    case Firewall:
        return "Firewall";
        break;
    case Headshot:
        return "Headshot";
        break;
    case Timer:
        return "Timer";
        break;
    default:
        return "";
    }
};
}
