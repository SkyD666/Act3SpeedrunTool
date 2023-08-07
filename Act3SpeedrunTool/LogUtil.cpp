#include "LogUtil.h"
#include <QCoreApplication>
#include <QDateTime>
#include <QDir>

Q_GLOBAL_STATIC(LogController, logControllerInstance)

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

void LogUtil::initLog()
{
    QDir logDir(getLogDir());
    if (!logDir.exists()) {
        logDir.mkpath(getLogDir());
    }
    if (firstTime) {
        if (logDir.entryInfoList(QStringList() << "*.log").size() > 10) {
            logDir.removeRecursively();
            logDir.mkpath(getLogDir());
        }
        firstTime = false;
    }
    logFile = new QFile(getLogFilePath(), this);
    logFile->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
}

void LogUtil::closeLog()
{
    if (logFile) {
        logFile->close();
        delete logFile;
        logFile = nullptr;
    }
}

void LogUtil::addLog(const QString newLog)
{
    QTextStream textStream(logFile);
    textStream << QDateTime::currentDateTime().toString("[yyyy-MM-dd hh:mm:ss] ") << newLog << "\n";
}

LogController* LogController::instance()
{
    return logControllerInstance;
}

LogController::LogController()
{
    open();
}

LogController::~LogController()
{
    innerCloseLog();
}

void LogController::addLog(const QString newLog)
{
    emit addLogSignal(newLog);
}

void LogController::open()
{
    QMutexLocker locker(&mutex);
    if (isOpened) {
        return;
    }
    worker = new LogUtil;
    workerThread = new QThread(this);
    worker->moveToThread(workerThread);
    connect(workerThread, &QThread::finished, worker, &QObject::deleteLater);
    connect(this, &LogController::addLogSignal, worker, &LogUtil::addLog);
    connect(this, &LogController::closeLog, worker, &LogUtil::closeLog);
    connect(this, &LogController::initLog, worker, &LogUtil::initLog);
    workerThread->start();
    emit initLog();
    isOpened = true;
}

void LogController::innerCloseLog()
{
    QMutexLocker locker(&mutex);
    if (!isOpened) {
        return;
    }
    emit closeLog();
    workerThread->quit();
    workerThread->wait();
    workerThread->deleteLater();
    isOpened = false;
}
