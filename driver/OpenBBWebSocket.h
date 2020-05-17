//
// Created by grant931 on 5/2/20.
//

#ifndef OPENBB_OPENBBWEBSOCKET_H
#define OPENBB_OPENBBWEBSOCKET_H
#include <QObject>
#include <QWebSocket>
#include <QByteArray>
#include <QMessageLogger>

class OpenBBWebSocket: public QObject {
    Q_OBJECT
public:
    OpenBBWebSocket(QWebSocket* webSocket, QMessageLogger* log);
public slots:
    void cleanUp();
    void receiveText(QString &message);
    void dispatchBinary(QByteArray bytes);
signals:
    void closing();
    void startStream();
    void sendText(QString &message);
    void streamAgain();
    void sendBinary(QByteArray bytes);
private:
    QWebSocket* webSocket;
    QMessageLogger* log;
};


#endif //OPENBB_OPENBBWEBSOCKET_H
