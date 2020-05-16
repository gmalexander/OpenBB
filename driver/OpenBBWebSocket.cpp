//
// Created by grant931 on 5/2/20.
//

#include "OpenBBWebSocket.h"

OpenBBWebSocket::OpenBBWebSocket(QWebSocket* webSocket) {
    this->webSocket = webSocket;
    OpenBBWebSocket::connect(this->webSocket, &QWebSocket::aboutToClose, this, &OpenBBWebSocket::cleanUp);
    OpenBBWebSocket::connect(this->webSocket, &QWebSocket::textMessageReceived, this, &OpenBBWebSocket::receiveText);
    OpenBBWebSocket::connect(this, &OpenBBWebSocket::sendText, this->webSocket, &QWebSocket::sendTextMessage);
    OpenBBWebSocket::connect(this, &OpenBBWebSocket::sendBinary, this->webSocket, &QWebSocket::sendBinaryMessage);
}

void OpenBBWebSocket::cleanUp() {
    emit this->closing();
}

void OpenBBWebSocket::receiveText(QString &message) {
    if(message == "START") {
        QString response{"STARTING"};
        emit this->startStream();
        emit this->sendText(response);
    }
    if (message == "MORE") {
        QString response{"STREAMING"};
        emit this->streamAgain();
        emit this->sendText(response);
    }
    if (message == "STOP") {
        QString response{"STOPPING"};
        emit this->closing();
        emit this->sendText(response);
    }
}

void OpenBBWebSocket::dispatchBinary(QByteArray bytes) {
    emit this->sendBinary(bytes);
}