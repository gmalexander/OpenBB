//
// Created by grant_ogz8xax on 3/19/2020.
//

#ifndef OPENBB_MARSHALLER_H
#define OPENBB_MARSHALLER_H
#include "BufferMeta.h"
#include "DriverStatus.h"
#include "ErrorOrigin.h"
#include <linux/videodev2.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <cerrno>
#include <QObject>
#include <QByteArray>
#include <QMessageLogger>

namespace OpenBB {
    class Marshaller : public QObject {

    Q_OBJECT

    public:

        Marshaller(QMessageLogger *log);

    public slots:

        void seed(BufferMeta *meta);

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

        void setDriverStatus(DriverStatus status);

        void binaryReady(QByteArray binaryData);

        void sendError(ErrorOrigin origin = DRIVER);

    private:

        BufferMeta *meta;

        QMessageLogger *log;

    };
}


#endif //OPENBB_MARSHALLER_H
