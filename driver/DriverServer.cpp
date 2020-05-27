//
// Created by grant931 on 5/2/20.
//

#include "DriverServer.h"
#include <string>

namespace OpenBB {
    DriverServer::DriverServer(int port, QString address) : QWebSocketServer("DriverServer",
                                                                             QWebSocketServer::SslMode::NonSecureMode) {
        this->log = new QMessageLogger("./log.txt", 0, "DriverServer");
        this->log->info("listening to port %d", port);
        this->listen(QHostAddress(address), port);
        this->setMaxPendingConnections(1);
        this->log->info("creating marshaller and requester");
        this->requester = new Requester(this->log);
        this->marshaller = new Marshaller(this->log);
        this->log->info("connecting seed and new connection handling");
        this->connectDriverErrorHandling();
        this->connectDriverStateful();
        this->connectTransitional();
    }

    void DriverServer::handleConnection() {
        this->log->info("pausing incoming connections");
        this->pauseAccepting();
        this->log->info("assigning new active socket");
        this->activeSocket = new WebSocket(this->nextPendingConnection(), this->log);
        this->log->info("connecting connection lifecycle handlers");
        this->connectSocketLifecycle();
    }

    void DriverServer::handleError(ErrorOrigin origin) {
        this->log->critical("Error reported from %s", toReadableOrigin(origin).toStdString().c_str());
        emit this->closing();
        emit this->resetting();
    }

    void DriverServer::reset() {
        auto statusMessage = toReadableStatus(this->status).toStdString().c_str();
        switch (this->status) {
            case STREAMING:
                this->log->info("Turning off stream at %s phase", statusMessage);
                emit this->unstreaming();
            case QUEUED:
            case UNSTREAM:
            case STREAM:
            case UNSTREAMED:
                this->log->info("Dequeuing driver at %s phase", statusMessage);
                emit this->dequeuing();
                break;
            default:
                this->log->info("No driver action taken for error at %s phase", statusMessage);
                break;
        }
        emit this->reopening();
    }

    void DriverServer::reopen() {
        this->log->info("resuming incoming connection acceptance");
        this->resumeAccepting();
    }

    void DriverServer::cleanUp() {
        this->log->info("disconnecting connection lifetime handlers");
        this->disconnectSocketLifecycle();
        this->log->info("deleting active socket");
        delete this->activeSocket;
        this->activeSocket = nullptr;
        emit this->resetting();
    }

    void DriverServer::setDriverStatus(DriverStatus status) {
        this->status = status;
    }

   //private members

    void DriverServer::connectDriverErrorHandling() {
        DriverServer::connect(this->requester, &Requester::sendError, this,
                              &DriverServer::handleError, Qt::QueuedConnection);
        DriverServer::connect(this->marshaller, &Marshaller::sendError, this,
                              &DriverServer::handleError, Qt::QueuedConnection);
        DriverServer::connect(this, &DriverServer::dequeuing, this->marshaller,
                              &Marshaller::dequeueBuffers, Qt::QueuedConnection);
    }

    void DriverServer::connectDriverStateful() {
        //a special case, we want driver statuses to be reported back as soon as possible
        DriverServer::connect(this->requester, &Requester::setDriverStatus, this,
                              &DriverServer::setDriverStatus);
        DriverServer::connect(this->marshaller, &Marshaller::setDriverStatus, this,
                              &DriverServer::setDriverStatus);
        DriverServer::connect(this, &DriverServer::resetting, this,
                              &DriverServer::reset, Qt::QueuedConnection);
    }

    void DriverServer::connectTransitional() {
        DriverServer::connect(this->requester, &Requester::buffersQueried, this->marshaller,
                              &Marshaller::seed, Qt::QueuedConnection);
        DriverServer::connect(this, &QWebSocketServer::newConnection, this,
                              &DriverServer::handleConnection,Qt::QueuedConnection);
        DriverServer::connect(this, &DriverServer::reopening, this,
                              &DriverServer::reopen, Qt::QueuedConnection);
    }

    void DriverServer::connectSocketLifecycle() {
        DriverServer::connect(this->activeSocket, &WebSocket::disconnected, this,
                              &DriverServer::cleanUp,Qt::QueuedConnection);
        DriverServer::connect(this->activeSocket, &WebSocket::startStream, this->requester,
                              &Requester::configureBuffers, Qt::QueuedConnection);
        DriverServer::connect(this->activeSocket, &WebSocket::streamAgain, this->marshaller,
                              &Marshaller::stream, Qt::QueuedConnection);
        DriverServer::connect(this->marshaller, &Marshaller::binaryReady, this->activeSocket,
                              &WebSocket::dispatchBinary, Qt::QueuedConnection);
        DriverServer::connect(this->activeSocket, &WebSocket::sendError, this,
                              &DriverServer::handleError, Qt::QueuedConnection);
        DriverServer::connect(this, &DriverServer::closing, this->activeSocket,
                              &WebSocket::close, Qt::QueuedConnection);
    }

    void DriverServer::disconnectSocketLifecycle() {
        DriverServer::disconnect(this->activeSocket, &WebSocket::disconnected, this,
                                 &DriverServer::cleanUp);
        DriverServer::disconnect(this->activeSocket, &WebSocket::startStream, this->requester,
                                 &Requester::configureBuffers);
        DriverServer::disconnect(this->activeSocket, &WebSocket::streamAgain, this->marshaller,
                                 &Marshaller::stream);
        DriverServer::disconnect(this->marshaller, &Marshaller::binaryReady, this->activeSocket,
                                 &WebSocket::dispatchBinary);
        DriverServer::disconnect(this->activeSocket, &WebSocket::sendError, this,
                                 &DriverServer::handleError);
        DriverServer::disconnect(this, &DriverServer::closing, this->activeSocket,
                                 &WebSocket::close);
    }
}