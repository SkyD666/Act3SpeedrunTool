#include "GlobalData.h"
#include "LanguageUtil.h"
#include "MainWindow.h"

#include <QApplication>
#include <QIcon>
#include <QTranslator>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    GlobalData::init();

    LanguageUtil::applyLanguage();

    QApplication::setWindowIcon(QIcon(":/icon.png"));
    QApplication::setOrganizationName("SkyD666");
    QApplication::setApplicationName(QObject::tr("末日三速通工具 Canary"));
    QApplication::setApplicationVersion("4.0-Canary");

    MainWindow w;
    w.show();
    return a.exec();
}
