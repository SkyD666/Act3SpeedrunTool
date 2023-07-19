#include "LanguageUtil.h"
#include "GlobalData.h"
#include <QCoreApplication>
#include <QLocale>

Q_GLOBAL_STATIC(LanguageUtil, instance)

QTranslator* LanguageUtil::translator = new QTranslator();

LanguageUtil::LanguageUtil()
{
}

LanguageUtil* LanguageUtil::getInstance()
{
    return instance;
}

QString LanguageUtil::getFileName(const QString name)
{
    for (auto l : languages) {
        if (l.name == name) {
            return l.fileName;
        }
    }
    return "";
}

QString LanguageUtil::getDisplayName(const QString name)
{
    if (name == "default") {
        return tr("跟随系统");
    } else if (name == "zh") {
        return tr("简体中文");
    } else if (name == "en") {
        return tr("英文");
    }
    return "";
}

void LanguageUtil::applyLanguage()
{
    bool setTranslator = false;
    if (!GlobalData::language.isEmpty()) {
        QString fileName = LanguageUtil::getInstance()->getFileName(GlobalData::language);
        if (fileName == "*") { // 简体中文无需更改
            setTranslator = true;
        } else if (!fileName.isEmpty() && translator->load(fileName, "./translations")) {
            QCoreApplication::installTranslator(translator);
            setTranslator = true;
        }
    }
    if (!setTranslator) {
        if (QLocale().language() != QLocale::Chinese) {
            if (translator->load(QLocale(), "", "", "./translations", ".qm")) {
                QCoreApplication::installTranslator(translator);
            }
        }
    }
}
