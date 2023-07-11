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
    QApplication::setApplicationName(QObject::tr("星神断网"));
    QApplication::setApplicationVersion("2.2");

    MainWindow w;
    w.show();
    return a.exec();
}
