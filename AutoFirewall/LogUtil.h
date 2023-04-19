#include <QString>

#pragma once

class LogUtil {
public:
    LogUtil();

    static QString getLogDir();

    static QString getLogFilePath();

    static void addLog(const QString newLog);

private:
    static QString logFileName;
};
