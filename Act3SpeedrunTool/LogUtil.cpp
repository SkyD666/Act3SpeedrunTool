#include "LogUtil.h"
#include <QCoreApplication>
#include <QDateTime>
#include <QDir>
#include <QFile>

QString LogUtil::logFileName = "Log - " + QDateTime::currentDateTime().toString("yyyy-MM-dd_hhmmss") + ".log";
bool LogUtil::firstTime = true;

LogUtil::LogUtil()
{
}

QString LogUtil::getLogDir()
{
    return QCoreApplication::applicationDirPath() + "/Logs";
}

QString LogUtil::getLogFilePath()
{
    return getLogDir() + "/" + logFileName;
}

void LogUtil::addLog(const QString newLog)
{
    QDir logDir(getLogDir());
    if (!logDir.exists()) {
        logDir.mkpath(getLogDir());
    }
    if (firstTime) {
        if (logDir.entryInfoList(QStringList() << "*.log").size() > 10) {
            logDir.removeRecursively();
        }
        firstTime = false;
    }
    QFile logFile(getLogFilePath());
    logFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
    QTextStream textStream(&logFile);
    textStream << QDateTime::currentDateTime().toString("[yyyy-MM-dd hh:mm:ss] ") << newLog << "\n";
    logFile.close();
}
