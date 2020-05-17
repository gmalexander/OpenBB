//
// Created by grant_ogz8xax on 3/19/2020.
//

#ifndef OPENBB_OPENBBREQUESTER_H
#define OPENBB_OPENBBREQUESTER_H
#include <linux/videodev2.h>
#include <QObject>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <QMessageLogger>
#include <fcntl.h>
#include "BufferMeta.h"
#include "OpenBBMarshaller.h"

class OpenBBRequester: public QObject {
    Q_OBJECT
public:
    explicit OpenBBRequester(QMessageLogger* log);
public slots:
    void configureBuffers();
    void requestBuffers();
    void queryBuffers();
signals:
    void buffersConfigured();
    void buffersRequested();
    void buffersQueried(BufferMeta* meta);
    void fatalRequesterError();
private:
    int fd;
    QMessageLogger *log;
};


#endif //OPENBB_OPENBBREQUESTER_H
