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

class OpenBBDriverServer: public QWebSocketServer {
  Q_OBJECT
public:
    OpenBBDriverServer(int port, QString address);
public slots:
    void handleConnection();
    void transmitData(std::vector<char> bytes);
    void cleanUp();
private:
    OpenBBWebSocket* activeSocket;
};


#endif //OPENBB_OPENBBDRIVERSERVER_H
