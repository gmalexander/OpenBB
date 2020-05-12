//
// Created by grant931 on 5/2/20.
//

#include "OpenBBDriverServer.h"

OpenBBDriverServer::OpenBBDriverServer(int port, QString address): QWebSocketServer("OpenBBDriverServer", QWebSocketServer::SslMode::NonSecureMode) {
    this->listen(QHostAddress(address), port);
    this->setMaxPendingConnections(1);
    OpenBBDriverServer::connect(this, &QWebSocketServer::newConnection, this, &OpenBBDriverServer::handleConnection);
}

void OpenBBDriverServer::handleConnection() {
    this->pauseAccepting();
    this->activeSocket = new OpenBBWebSocket(this->nextPendingConnection());
    OpenBBDriverServer::connect(this->activeSocket, &OpenBBWebSocket::closing, this, &OpenBBDriverServer::cleanUp);
}

void OpenBBDriverServer::cleanUp() {
    OpenBBDriverServer::disconnect(this->activeSocket, &OpenBBWebSocket::closing, this, &OpenBBDriverServer::cleanUp);
    delete this->activeSocket;
    this->activeSocket = nullptr;
    this->resumeAccepting();
}