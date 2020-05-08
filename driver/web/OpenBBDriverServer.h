//
// Created by grant931 on 5/2/20.
//

#ifndef OPENBB_OPENBBDRIVERSERVER_H
#define OPENBB_OPENBBDRIVERSERVER_H
#include <QWebSocketServer>
#include <QString>
#include <QHostAddress>
#include <vector>
#include "OpenBBWebSocket.h"

class OpenBBDriverServer: public QObject {
  Q_OBJECT
public:
    OpenBBDriverServer(int port, QString address);
private:
    QWebSocketServer* server;
    QHostAddress address;
    int port;
    std::vector<QWebSocket> sockets;
};


#endif //OPENBB_OPENBBDRIVERSERVER_H
