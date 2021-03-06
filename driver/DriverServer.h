//
// Created by grant931 on 5/2/20.
//

#ifndef OPENBB_DRIVERSERVER_H
#define OPENBB_DRIVERSERVER_H
#include <QWebSocketServer>
#include <QString>
#include <QHostAddress>
#include <QMessageLogger>
#include "WebSocket.h"
#include "hardware/Marshaller.h"
#include "hardware/Requester.h"

namespace OpenBB {
    class DriverServer : public QWebSocketServer {

    Q_OBJECT

    public:

        DriverServer(int port, QString address);

    public slots:

        void handleConnection();

        void cleanUp();

        void setDriverStatus(DriverStatus status);

        void handleError(ErrorOrigin error);

        void reset();

        void reopen();

    signals:

        void dequeuing();

        void closing();

        void resetting();

        void reopening();

        void unstreaming();

    private:
        void connectDriverErrorHandling();

        void connectDriverStateful();

        void connectTransitional();

        void connectSocketLifecycle();

        void disconnectSocketLifecycle();

        QMessageLogger *log;

        WebSocket *activeSocket;

        Requester *requester;

        Marshaller *marshaller;

        DriverStatus status;
    };
}

#endif //OPENBB_DRIVERSERVER_H
