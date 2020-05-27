//
// Created by grant_ogz8xax on 3/19/2020.
//

#ifndef OPENBB_REQUESTER_H
#define OPENBB_REQUESTER_H
#include <linux/videodev2.h>
#include <QObject>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <QMessageLogger>
#include <fcntl.h>
#include "BufferMeta.h"
#include "DriverStatus.h"
#include "ErrorOrigin.h"

namespace OpenBB {
    class Requester : public QObject {
    Q_OBJECT
    public:
        explicit Requester(QMessageLogger *log);

    public slots:

        void configureBuffers();

        void requestBuffers();

        void queryBuffers();

    signals:

        void setDriverStatus(DriverStatus status);

        void buffersConfigured();

        void buffersRequested();

        void buffersQueried(BufferMeta *meta);

        void sendError(ErrorOrigin origin = DRIVER);

    private:
        int fd;
        QMessageLogger *log;
    };
}

#endif //OPENBB_REQUESTER_H
