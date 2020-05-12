//
// Created by grant931 on 5/2/20.
//

#ifndef OPENBB_OPENBBWEBSOCKET_H
#define OPENBB_OPENBBWEBSOCKET_H
#include <QObject>
#include <QWebSocket>

class OpenBBWebSocket: public QObject {
    Q_OBJECT
public:
    OpenBBWebSocket(QWebSocket* webSocket);

public slots:
    void cleanUp();
signals:
    void closing();
private:
    QWebSocket* webSocket;
};


#endif //OPENBB_OPENBBWEBSOCKET_H
