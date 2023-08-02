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

    // 必须设置为 false。否则在没有顶层窗口或者隐藏主窗口后，如果有子窗口弹出等，若关闭该弹出的子窗口，就会导致主窗口也被关闭
    QApplication::setQuitOnLastWindowClosed(false);
    QApplication::setWindowIcon(QIcon("://image/ic_icon.png"));
    QApplication::setOrganizationName("SkyD666");
    QApplication::setApplicationName(QObject::tr("末日三速通工具"));
    QApplication::setApplicationVersion("5.0");

    qApp->setStyle(GlobalData::styleName);

    if (QApplication::applicationVersion() != GlobalData::version) {
        QMessageBox::warning(nullptr, QString(), QObject::tr("*** 应用版本变更，请认真检查各项配置（热键、外观、提示音等）是否需要重新设置！***"));
    }

    MainWindow w;
    w.show();
    return a.exec();
}
