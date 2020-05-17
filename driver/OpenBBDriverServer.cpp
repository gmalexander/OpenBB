//
// Created by grant931 on 5/2/20.
//

#include "OpenBBDriverServer.h"
#include <string>

OpenBBDriverServer::OpenBBDriverServer(int port, QString address): QWebSocketServer("OpenBBDriverServer", QWebSocketServer::SslMode::NonSecureMode) {
    this->log = new QMessageLogger("./log.txt", 0, "OpenBBDriverServer");
    this->log->info("listening to port %d", port);
    this->listen(QHostAddress(address), port);
    this->setMaxPendingConnections(1);
    this->log->info("creating marshaller and requester");
    this->requester = new OpenBBRequester(this->log);
    this->marshaller = new OpenBBMarshaller(this->log);
    this->log->info("connecting seed and new connection handling");
    OpenBBDriverServer::connect(this->requester, &OpenBBRequester::buffersQueried, this->marshaller, &OpenBBMarshaller::seed);
    OpenBBDriverServer::connect(this->requester, &OpenBBRequester::fatalRequesterError, this, &OpenBBDriverServer::cleanUp);
    OpenBBDriverServer::connect(this->marshaller, &OpenBBMarshaller::fatalMarshallerError, this, &OpenBBDriverServer::cleanUp);
    OpenBBDriverServer::connect(this, &QWebSocketServer::newConnection, this, &OpenBBDriverServer::handleConnection);
}

void OpenBBDriverServer::handleConnection() {
    this->log->info("pausing incoming connections");
    this->pauseAccepting();
    this->log->info("assigning new active socket");
    this->activeSocket = new OpenBBWebSocket(this->nextPendingConnection(), this->log);
    this->log->info("connecting connection lifetime handlers");
    OpenBBDriverServer::connect(this->activeSocket, &OpenBBWebSocket::closing, this, &OpenBBDriverServer::cleanUp);
    OpenBBDriverServer::connect(this->activeSocket, &OpenBBWebSocket::startStream, this->requester, &OpenBBRequester::configureBuffers);
    OpenBBDriverServer::connect(this->activeSocket, &OpenBBWebSocket::streamAgain, this->marshaller, &OpenBBMarshaller::stream);
    OpenBBDriverServer::connect(this->marshaller, &OpenBBMarshaller::binaryReady, this->activeSocket, &OpenBBWebSocket::dispatchBinary);
}

void OpenBBDriverServer::cleanUp() {
    this->log->info("disconnecting connection lifetime handlers");
    OpenBBDriverServer::disconnect(this->activeSocket, &OpenBBWebSocket::closing, this, &OpenBBDriverServer::cleanUp);
    OpenBBDriverServer::disconnect(this->activeSocket, &OpenBBWebSocket::closing, this, &OpenBBDriverServer::cleanUp);
    OpenBBDriverServer::disconnect(this->activeSocket, &OpenBBWebSocket::startStream, this->requester, &OpenBBRequester::configureBuffers);
    OpenBBDriverServer::disconnect(this->activeSocket, &OpenBBWebSocket::streamAgain, this->marshaller, &OpenBBMarshaller::stream);
    OpenBBDriverServer::disconnect(this->marshaller, &OpenBBMarshaller::binaryReady, this->activeSocket, &OpenBBWebSocket::dispatchBinary);
    this->log->info("deleting active socket");
    delete this->activeSocket;
    this->activeSocket = nullptr;
    this->log->info("resuming incoming connection acceptance");
    this->resumeAccepting();
    emit this->closeDriver();
}