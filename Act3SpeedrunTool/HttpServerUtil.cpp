#include "HttpServerUtil.h"
#include "GlobalData.h"

#include <QApplication>
#include <QDataStream>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkInterface>

Q_GLOBAL_STATIC(HttpServerController, controllerInstance)

quint16 HttpServerUtil::currentHttpPort = 0;
quint16 HttpServerUtil::currentWebsocketPort = 0;

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
    httpServer->route("/favicon.ico", [](QHttpServerResponder&& responder) {
        auto favicon = QFile("./resource/favicon.ico");
        favicon.open(QFile::ReadOnly);
        responder.write(favicon.readAll(), "image/x-icon");
    });
    httpServer->route("/displayInfo", []() {
        auto htmlFile = QFile("./resource/DisplayInfo.html");
        htmlFile.open(QFile::ReadOnly | QFile::Text);
        return QTextStream(&htmlFile).readAll();
    });
    httpServer->route("/displayInfo.js", []() {
        auto jsFile = QFile("./resource/DisplayInfo.js");
        jsFile.open(QFile::ReadOnly | QFile::Text);
        return QTextStream(&jsFile).readAll();
    });
    currentHttpPort = httpServer->listen(QHostAddress::Any, GlobalData::serverHttpPort);
    if (webSocketServer->listen(QHostAddress::Any, GlobalData::serverWebsocketPort)) {
        currentWebsocketPort = GlobalData::serverWebsocketPort;
        connect(webSocketServer, &QWebSocketServer::newConnection, this, &HttpServerUtil::onNewConnection);
    }
}

void HttpServerUtil::stopHttp()
{
    webSocketServer->close();

    httpServer->deleteLater();
    webSocketServer->deleteLater();
}

void HttpServerUtil::startTimer(bool isContinue, qint64 startTimestamp)
{
    this->startTimestamp = startTimestamp;
    timerState = TimerState(TimerState::Running | (isContinue ? 0 : TimerState::Zero));
    sendNewData(getTimerStateJson(timerState, startTimestamp));
}

void HttpServerUtil::stopTimer()
{
    timerState = TimerState::Stopped;
    sendNewData(getTimerStateJson(timerState, startTimestamp));
}

void HttpServerUtil::pauseTimer()
{
    timerState = TimerState::Paused;
    sendNewData(getTimerStateJson(timerState, startTimestamp));
}

void HttpServerUtil::zeroTimer()
{
    timerState = TimerState::Zero;
    sendNewData(getTimerStateJson(timerState, startTimestamp));
}

void HttpServerUtil::sendNewData()
{
    for (auto c : clients) {
        c->sendTextMessage(getHeadshotJson(&data).toJson(QJsonDocument::Compact));
    }
}

void HttpServerUtil::sendNewData(QJsonDocument json)
{
    QString jsonString = json.toJson(QJsonDocument::Compact);
    for (auto c : clients) {
        c->sendTextMessage(jsonString);
    }
}

void HttpServerUtil::sendNewData(short headshotCount)
{
    data.headshotCount = headshotCount;
    sendNewData();
}

void HttpServerUtil::sendNewData(QWebSocket* webSocket)
{
    webSocket->sendTextMessage(getHeadshotJson(&data).toJson(QJsonDocument::Compact));
    webSocket->sendTextMessage(getTimerStateJson(timerState, startTimestamp).toJson(QJsonDocument::Compact));
}

QString HttpServerUtil::getHttpServerDomain()
{
    const QHostAddress& localhost = QHostAddress(QHostAddress::LocalHost);
    for (const QHostAddress& address : QNetworkInterface::allAddresses()) {
        QString addressString = address.toString();
        if (address.protocol() == QAbstractSocket::IPv4Protocol
            && address != localhost
            && addressString.section(".", -1, -1) != "1") {
            return QString("http://%1:%2/displayInfo").arg(addressString, QString::number(currentHttpPort));
        }
    }
    return "";
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

QJsonDocument HttpServerUtil::getHeadshotJson(DataPackage* data)
{
    QJsonObject object {
        { "headshotCount", data->headshotCount },
    };
    return QJsonDocument(object);
}

QJsonDocument HttpServerUtil::getTimerStateJson(TimerState state, qint64 startTimestamp)
{
    QJsonObject object {
        { "timerState", state },
        { "startTimestamp", startTimestamp },
        { "serverTimestamp", QDateTime::currentDateTime().toMSecsSinceEpoch() },
    };
    return QJsonDocument(object);
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
    connect(this, &HttpServerController::startOrContinueTimer, worker, QOverload<bool, qint64>::of(&HttpServerUtil::startTimer));
    connect(this, &HttpServerController::stopTimer, worker, &HttpServerUtil::stopTimer);
    connect(this, &HttpServerController::pauseTimer, worker, &HttpServerUtil::pauseTimer);
    connect(this, &HttpServerController::zeroTimer, worker, &HttpServerUtil::zeroTimer);
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
