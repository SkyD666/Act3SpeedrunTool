#include "GlobalData.h"
#include "MainWindow.h"

#include <QApplication>
#include <QIcon>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    QApplication::setWindowIcon(QIcon(":/icon.png"));
    QApplication::setOrganizationName("SkyD666");
    QApplication::setApplicationName("星神断网");
    QApplication::setApplicationVersion("1.3");
    GlobalData::init();
    MainWindow w;
    w.show();
    bool result = a.exec();
    GlobalData::destory();
    return result;
}
