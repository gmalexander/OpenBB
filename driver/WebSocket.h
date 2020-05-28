//
// Created by grant931 on 5/2/20.
//

#ifndef OPENBB_WEBSOCKET_H
#define OPENBB_WEBSOCKET_H
#include <QObject>
#include <QWebSocket>
#include <QByteArray>
#include <QMessageLogger>
#include "hardware/ErrorOrigin.h"

namespace OpenBB {
    class WebSocket : public QObject {

    Q_OBJECT

    public:

        WebSocket(QWebSocket *webSocket, QMessageLogger *log);

    public slots:

        void receiveText(QString &message);

        void dispatchBinary(QByteArray bytes);

        void close();

    signals:

        void disconnected();

        void startStream();

        void sendText(QString &message);

        void closing();

        void streamAgain();

        void sendBinary(QByteArray bytes);

        void sendError(ErrorOrigin origin = SOCKET);

    private:

        QWebSocket *webSocket;

        QMessageLogger *log;

    };
}

#endif //OPENBB_WEBSOCKET_H
