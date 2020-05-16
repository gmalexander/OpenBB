//
// Created by grant931 on 5/2/20.
//

#ifndef OPENBB_OPENBBDRIVERSERVER_H
#define OPENBB_OPENBBDRIVERSERVER_H
#include <QWebSocketServer>
#include <QString>
#include <QHostAddress>
#include "OpenBBWebSocket.h"
#include "../hardware/OpenBBMarshaller.h"
#include "../hardware/OpenBBRequester.h"

class OpenBBDriverServer: public QWebSocketServer {
  Q_OBJECT
public:
    OpenBBDriverServer(int port, QString address);
public slots:
    void handleConnection();
    void cleanUp();
private:
    OpenBBWebSocket* activeSocket;
    OpenBBRequester* requester;
    OpenBBMarshaller* marshaller;
};


#endif //OPENBB_OPENBBDRIVERSERVER_H
