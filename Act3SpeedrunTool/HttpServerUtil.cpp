#include "HttpServerUtil.h"

#include <QApplication>
#include <QJsonDocument>
#include <QJsonObject>
#include <QtConcurrent>

Q_GLOBAL_STATIC(HttpServerController, controllerInstance)

HttpServerUtil::HttpServerUtil()
{
}

HttpServerUtil::~HttpServerUtil()
{
}

void HttpServerUtil::startHttp()
{
    if (started) {
        return;
    }
    started = true;
    httpServer = new QHttpServer(this);
    webSocketServer = new QWebSocketServer("Act3 Speedrun Tool", QWebSocketServer::NonSecureMode, this);
    httpServer->route("/displayInfo", []() {
        return "Example server. Please see documentation for API description";
    });
    port = httpServer->listen(QHostAddress::Any, 9975);
    if (webSocketServer->listen(QHostAddress::Any, 9976)) {
        connect(webSocketServer, &QWebSocketServer::newConnection, this, &HttpServerUtil::onNewConnection);
    }
}

void HttpServerUtil::stopHttp()
{
    webSocketServer->close();

    httpServer->deleteLater();
    webSocketServer->deleteLater();
}

void HttpServerUtil::sendNewData()
{
    for (auto c : clients) {
        c->sendTextMessage(getJson(&data));
    }
}

void HttpServerUtil::sendNewData(short headshotCount)
{
    data.headshotCount = headshotCount;
    sendNewData();
}

void HttpServerUtil::sendNewData(QWebSocket* webSocket)
{
    webSocket->sendTextMessage(getJson(&data));
}

void HttpServerUtil::onNewConnection()
{
    QWebSocket* pSocket = webSocketServer->nextPendingConnection();

    //    connect(pSocket, &QWebSocket::textMessageReceived, this, &EchoServer::processTextMessage);
    //    connect(pSocket, &QWebSocket::binaryMessageReceived, this, &EchoServer::processBinaryMessage);
    connect(pSocket, &QWebSocket::disconnected, this, &HttpServerUtil::socketDisconnected);

    clients << pSocket;

    sendNewData(pSocket);
}

void HttpServerUtil::onCloseConnection()
{
}

void HttpServerUtil::socketDisconnected()
{
    QWebSocket* pClient = qobject_cast<QWebSocket*>(sender());
    if (pClient) {
        clients.removeAll(pClient);
        pClient->deleteLater();
    }
}

QString HttpServerUtil::getJson(DataPackage* data)
{
    QJsonObject object {
        { "headshotCount", data->headshotCount },
    };
    QJsonDocument doc(object);
    return doc.toJson(QJsonDocument::Compact);
}

HttpServerController* HttpServerController::getInstance()
{
    return controllerInstance;
}

HttpServerController::HttpServerController()
{
}

HttpServerController::~HttpServerController()
{
    stop();
}

void HttpServerController::start()
{
    QMutexLocker locker(&mutex);
    if (started) {
        return;
    }
    worker = new HttpServerUtil;
    workerThread = new QThread;
    worker->moveToThread(workerThread);
    connect(workerThread, &QThread::finished, worker, &QObject::deleteLater);
    connect(this, &HttpServerController::sendNewData, worker, QOverload<short>::of(&HttpServerUtil::sendNewData));
    connect(this, &HttpServerController::stopHttp, worker, &HttpServerUtil::stopHttp);
    connect(this, &HttpServerController::initHttpServerUtil, worker, &HttpServerUtil::startHttp);
    workerThread->start();
    emit initHttpServerUtil();
    started = true;
}

void HttpServerController::stop()
{
    QMutexLocker locker(&mutex);
    if (!started) {
        return;
    }
    emit stopHttp();
    workerThread->quit();
    workerThread->wait();
    started = false;
}
