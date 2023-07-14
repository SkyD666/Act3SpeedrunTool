#include "SubFunction.h"
#include <QColor>
#include <QFont>
#include <QObject>
#include <QPointF>

namespace SubFunctionUtil {

QString toDisplayString(SubFunction f)
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

QString toString(SubFunction f)
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
