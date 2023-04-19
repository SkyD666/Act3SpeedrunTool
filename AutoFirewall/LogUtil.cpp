#include "LogUtil.h"
#include <QCoreApplication>
#include <QDateTime>
#include <QDir>
#include <QFile>

QString LogUtil::logFileName = "Log - " + QDateTime::currentDateTime().toString("yyyy-MM-dd_hhmmss") + ".log";

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
    QFile logFile(getLogFilePath());
    logFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
    QTextStream textStream(&logFile);
    textStream << QDateTime::currentDateTime().toString("[yyyy-MM-dd hh:mm:ss] ") << newLog << "\n";
    logFile.close();
}
