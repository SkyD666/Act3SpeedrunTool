#include "GlobalData.h"
#include "LanguageUtil.h"
#include "MainWindow.h"

#include <QApplication>
#include <QIcon>
#include <QMessageBox>
#include <QTranslator>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    GlobalData::init();

    LanguageUtil::applyLanguage();

    QApplication::setWindowIcon(QIcon(":/icon.png"));
    QApplication::setOrganizationName("SkyD666");
    QApplication::setApplicationName(QObject::tr("末日三速通工具"));
    QApplication::setApplicationVersion("4.0-beta01");

    if (QApplication::applicationVersion() != GlobalData::version) {
        QMessageBox::critical(nullptr, QString(), QObject::tr("*** 应用版本变更，请认真检查各项配置（热键、外观、提示音等）是否需要重新设置！***"));
    }

    MainWindow w;
    w.show();
    return a.exec();
}
