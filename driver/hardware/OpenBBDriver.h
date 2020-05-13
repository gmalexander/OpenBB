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
    void stream();
signals:
    void binaryDataReady(std::vector<char> data);
private:
    OpenBBRequester* requester;
};


#endif //OPENBB_OPENBBDRIVER_H
