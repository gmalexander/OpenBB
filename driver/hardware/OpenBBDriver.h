//
// Created by grant_ogz8xax on 3/19/2020.
//

#ifndef OPENBB_OPENBBDRIVER_H
#define OPENBB_OPENBBDRIVER_H
#include <QObject>
#include <vector>
#include "OpenBBMarshaller.h"
#include "OpenBBRequester.h"

class OpenBBDriver: public QObject {
    Q_OBJECT
public:
    OpenBBDriver();
public slots:
    void startUp();
    void reload();
    void receiveBinary();
    void closeDown();
signals:
    void start();
    void readyForMore();
    void binaryReceived();
    void stop();
private:
    OpenBBRequester* requester;
    OpenBBMarshaller* marshaller;
};


#endif //OPENBB_OPENBBDRIVER_H
