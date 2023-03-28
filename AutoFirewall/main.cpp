#include "GlobalData.h"
#include "MainWindow.h"

#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    QApplication::setOrganizationName("SkyD666");
    QApplication::setApplicationName("AutoFirewall");
    QApplication::setApplicationVersion("1.1");
    GlobalData::init();
    MainWindow w;
    w.show();
    bool result = a.exec();
    GlobalData::destory();
    return result;
}
