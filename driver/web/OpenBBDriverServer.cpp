//
// Created by grant931 on 5/2/20.
//

#include "OpenBBDriverServer.h"

OpenBBDriverServer::OpenBBDriverServer(int port, QString address): address{address}, port{port} {
    QHostAddress addr(address);
    this->server = new QWebSocketServer("OpenBBDriverServer", QWebSocketServer::SslMode::NonSecureMode);
    this->connect(this->server, &QWebSocketServer::newConnection, this, &OpenBBDriverServer::handleNewConnection);
}

void OpenBBDriverServer::start() {
    this->server->listen(this->address, this->port);
}

void OpenBBDriverServer::handleNewConnection() {
    
}