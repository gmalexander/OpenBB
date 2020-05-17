//
// Created by grant931 on 5/2/20.
//

#include "OpenBBWebSocket.h"

OpenBBWebSocket::OpenBBWebSocket(QWebSocket* webSocket, QMessageLogger* log) {
    this->webSocket = webSocket;
    this->log = log;
    this->log->info("connecting clean up and message receivers");
    OpenBBWebSocket::connect(this->webSocket, &QWebSocket::aboutToClose, this, &OpenBBWebSocket::cleanUp);
    OpenBBWebSocket::connect(this->webSocket, &QWebSocket::textMessageReceived, this, &OpenBBWebSocket::receiveText);
    this->log->info("connecting output slots and signals");
    OpenBBWebSocket::connect(this, &OpenBBWebSocket::sendText, this->webSocket, &QWebSocket::sendTextMessage);
    OpenBBWebSocket::connect(this, &OpenBBWebSocket::sendBinary, this->webSocket, &QWebSocket::sendBinaryMessage);
}

void OpenBBWebSocket::cleanUp() {
    this->log->info("Websocket closing, emitting");
    emit this->closing();
}

void OpenBBWebSocket::receiveText(QString &message) {
    if(message == "START") {
        this->log->info("Message START received");
        QString response{"STARTING"};
        this->log->info("starting stream");
        emit this->startStream();
        this->log->info("sending back response STARTING");
        emit this->sendText(response);
    }
    if (message == "MORE") {
        this->log->info("Message STREAMING received");
        QString response{"STREAMING"};
        this->log->info("Resuming streaming");
        emit this->streamAgain();
        this->log->info("sending back response STREAMING");
        emit this->sendText(response);
    }
    if (message == "STOP") {
        this->log->info("Message STOP received");
        QString response{"STOPPING"};
        this->log->info("Closing stream");
        emit this->closing();
        this->log->info("sending back response STOPPING");
        emit this->sendText(response);
    }
}

void OpenBBWebSocket::dispatchBinary(QByteArray bytes) {
    this->log->info("dispatching binary");
    emit this->sendBinary(bytes);
}