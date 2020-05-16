//
// Created by grant_ogz8xax on 3/19/2020.
//

#ifndef OPENBB_OPENBBMARSHALLER_H
#define OPENBB_OPENBBMARSHALLER_H
#include "BufferMeta.h"
#include <linux/videodev2.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <cerrno>
#include <QObject>
#include <cstring>
#include <iostream>
#include <QByteArray>

class OpenBBMarshaller: public QObject {
    Q_OBJECT
public:
    OpenBBMarshaller();
public slots:
    void seed(BufferMeta* meta);
    void queueBuffers();
    void dequeueBuffers();
    void stream();
    void unstream();
    void createBinary();
signals:
    void seeded();
    void buffersQueued();
    void streaming();
    void stoppedStreaming();
    void buffersDequeued();
    void binaryReady(QByteArray binaryData);
private:
    BufferMeta* meta;
};


#endif //OPENBB_OPENBBMARSHALLER_H
