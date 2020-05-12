//
// Created by grant931 on 5/2/20.
//

#include "OpenBBWebSocket.h"

OpenBBWebSocket::OpenBBWebSocket(QWebSocket* webSocket) {
    this->webSocket = webSocket;
    OpenBBWebSocket::connect(this->webSocket, &QWebSocket::aboutToClose, this, &OpenBBWebSocket::cleanUp);
}

void OpenBBWebSocket::cleanUp() {
    emit this->closing();
}

