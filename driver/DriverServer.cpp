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
        this->connectDriverTransitional();
    }

    void DriverServer::handleConnection() {
        this->log->info("pausing incoming connections");
        this->pauseAccepting();
        this->log->info("assigning new active socket");
        this->activeSocket = new WebSocket(this->nextPendingConnection(), this->log);
        this->log->info("connecting connection lifecycle handlers");
        this->connectSocketLifecycle();
    }

    void DriverServer::handleError() {
        switch (this->status) {
            case QUEUED:
                this->log->critical("Dequeuing driver following error at QUEUED phase");
                emit this->dequeueDriver();
                break;
            case STREAM:
                this->log->critical("Dequeuing driver following error at STREAM phase");
                emit this->dequeueDriver();
                break;
            case STREAMING:
                this->log->critical("Turning off stream and dequeuing following error at STREAMING phase");
                emit this->unstreamDriver();
                emit this->dequeueDriver();
                break;
            case UNSTREAM:
                this->log->critical("Dequeuing following error at UNSTREAM phase");
                emit this->dequeueDriver();
                break;
            case UNSTREAMED:
                this->log->critical("Dequeuing follow error at UNSTREAMED phase");
                emit this->dequeueDriver();
                break;
            default:
                this->log->critical("No driver action taken for error at %s phase", toReadableStatus(status).toStdString().c_str());
                break;
        }
    }

    void DriverServer::cleanUp() {
        this->log->info("disconnecting connection lifetime handlers");
        this->disconnectSocketLifecycle();
        this->log->info("deleting active socket");
        delete this->activeSocket;
        this->activeSocket = nullptr;
        this->log->info("resuming incoming connection acceptance");
        emit this->resetDriver();
    }

    void DriverServer::setDriverStatus(DriverStatus status) {
        this->status = status;
    }

   //private members

    void DriverServer::connectDriverErrorHandling() {
        DriverServer::connect(this->requester, &Requester::fatalRequesterError, this,
                              &DriverServer::handleError, Qt::QueuedConnection);
        DriverServer::connect(this->marshaller, &Marshaller::fatalMarshallerError, this,
                              &DriverServer::handleError, Qt::QueuedConnection);
        DriverServer::connect(this, &DriverServer::dequeueDriver, this->marshaller,
                              &Marshaller::dequeueBuffers, Qt::QueuedConnection);
    }

    void DriverServer::connectDriverStateful() {
        //a special case, we want driver statuses to be reported back as soon as possible
        DriverServer::connect(this->requester, &Requester::setDriverStatus, this,
                              &DriverServer::setDriverStatus);
        DriverServer::connect(this->marshaller, &Marshaller::setDriverStatus, this,
                              &DriverServer::setDriverStatus);
    }

    void DriverServer::connectDriverTransitional() {
        DriverServer::connect(this->requester, &Requester::buffersQueried, this->marshaller,
                              &Marshaller::seed, Qt::QueuedConnection);
        DriverServer::connect(this, &QWebSocketServer::newConnection, this, &DriverServer::handleConnection,
                              Qt::QueuedConnection);
    }

    void DriverServer::connectSocketLifecycle() {
        DriverServer::connect(this->activeSocket, &WebSocket::closing, this, &DriverServer::cleanUp,
                              Qt::QueuedConnection);
        DriverServer::connect(this->activeSocket, &WebSocket::startStream, this->requester,
                              &Requester::configureBuffers, Qt::QueuedConnection);
        DriverServer::connect(this->activeSocket, &WebSocket::streamAgain, this->marshaller,
                              &Marshaller::stream, Qt::QueuedConnection);
        DriverServer::connect(this->marshaller, &Marshaller::binaryReady, this->activeSocket,
                              &WebSocket::dispatchBinary, Qt::QueuedConnection);
    }

    void DriverServer::disconnectSocketLifecycle() {
        DriverServer::disconnect(this->activeSocket, &WebSocket::closing, this,
                                 &DriverServer::cleanUp);
        DriverServer::disconnect(this->activeSocket, &WebSocket::startStream, this->requester,
                                 &Requester::configureBuffers);
        DriverServer::disconnect(this->activeSocket, &WebSocket::streamAgain, this->marshaller,
                                 &Marshaller::stream);
        DriverServer::disconnect(this->marshaller, &Marshaller::binaryReady, this->activeSocket,
                                 &WebSocket::dispatchBinary);
    }
}