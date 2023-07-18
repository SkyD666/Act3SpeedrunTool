#pragma once

#include <QGlobalStatic>
#include <QHttpServer>
#include <QObject>
#include <QWebSocketServer>

class HttpServerUtil : public QObject {
    Q_OBJECT

signals:
    void send(short headshotCount);

public:
    struct DataPackage {
        short headshotCount = 0;
    };

    HttpServerUtil();
    ~HttpServerUtil();

    void startHttp();
    void stopHttp();

    void sendNewData();

    void sendNewData(short headshotCount);

    void sendNewData(QWebSocket* webSocket);

protected:
    void onNewConnection();

    void onCloseConnection();

    void socketDisconnected();

    QString getJson(DataPackage* data);

private:
    bool started = false;

    quint16 port = 9975;

    QHttpServer* httpServer = nullptr;

    QWebSocketServer* webSocketServer = nullptr;

    QList<QWebSocket*> clients;

    DataPackage data;
};

class HttpServerController : public QObject {
    Q_OBJECT

public:
    static HttpServerController* getInstance();

    HttpServerController();
    ~HttpServerController();

    void start();

    void stop();

signals:
    void sendNewData(short headshotCount);

    void stopHttp();

    void initHttpServerUtil();

private:
    QMutex mutex;

    bool started = false;

    HttpServerUtil* worker = nullptr;

    QThread* workerThread = nullptr;
};
