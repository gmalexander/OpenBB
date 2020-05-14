//
// Created by grant_ogz8xax on 3/19/2020.
//

#ifndef OPENBB_OPENBBREQUESTER_H
#define OPENBB_OPENBBREQUESTER_H
#include <vector>
#include <iostream>
#include <linux/videodev2.h>
#include <QObject>
#include <sys/ioctl.h>
#include <cstring>
#include <sys/mman.h>
#include <fcntl.h>
#include "BufferMeta.h"
#include "OpenBBMarshaller.h"

class OpenBBRequester: public QObject {
    Q_OBJECT
public:
    explicit OpenBBRequester(int fd);
public slots:
    void configureBuffers();
    void requestBuffers();
    void queryBuffers();
signals:
    void buffersConfigured();
    void buffersRequested();
    void buffersQueried(BufferMeta* meta);
private:
    int fd;
};


#endif //OPENBB_OPENBBREQUESTER_H
