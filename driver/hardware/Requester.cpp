//
// Created by grant_ogz8xax on 3/19/2020.
//

#include "Requester.h"

namespace OpenBB {
    Requester::Requester(QMessageLogger *log) : fd{open("/dev/video0", O_RDWR)} {
        this->log = log;
        this->log->info("connecting request lifecycle");
        Requester::connect(this, &Requester::buffersConfigured, this, &Requester::requestBuffers,
                           Qt::QueuedConnection);
        Requester::connect(this, &Requester::buffersRequested, this, &Requester::queryBuffers,
                           Qt::QueuedConnection)
    }

    void Requester::configureBuffers() {
        emit this->setDriverStatus(CONFIGURING);
        this->log->info("configuring buffers for 800x600 and mjpeg");
        v4l2_format format;
        format.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG;
        format.fmt.pix.height = 800;
        format.fmt.pix.width = 600;
        int result = ioctl(this->fd, VIDIOC_S_FMT, &format);
        if (result != 0) {
            this->log->critical("ioctl returned invalid value: %d", result);
            this->log->critical("setting format failed for reason: %s", strerror(errno));
            emit this->sendError();
            return;
        }
        this->log->info("reporting buffers to have been configured");
        emit this->setDriverStatus(CONFIGURED);
        emit this->buffersConfigured();
    }

    void Requester::requestBuffers() {
        emit this->setDriverStatus(REQUESTING);
        this->log->info("requesting 1 buffer of type video capture in memory map");
        v4l2_requestbuffers reqbuf;
        reqbuf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        reqbuf.count = 1;
        reqbuf.memory = V4L2_MEMORY_MMAP;
        int result = ioctl(this->fd, VIDIOC_REQBUFS, &reqbuf);
        if (result != 0) {
            this->log->critical("ioctl returned invalid value: %d", result);
            this->log->critical("requesting buffers failed for reason: %s", strerror(errno));
            emit this->sendError();
            return;
        }
        this->log->info("reporting buffers to have been requested");
        emit this->setDriverStatus(REQUESTED);
        emit this->buffersRequested();
    }

    void Requester::queryBuffers() {
        emit this->setDriverStatus(QUERYING);
        this->log->info("querying buffers for type video capture at index 0");
        v4l2_buffer buf;
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.index = 0;
        int result = ioctl(this->fd, VIDIOC_QUERYBUF, &buf);
        if (result != 0) {
            this->log->critical("iotctl returned invalid value: %d", result);
            this->log->critical("querying buffers failed for reason: %s", strerror(errno));
            emit this->sendError();
            return;
        }
        this->log->info("reporting buffers to have been queried, and sending back buffer of length %d and offset %d",
                        buf.length, buf.m.offset);
        emit this->setDriverStatus(QUERIED);
        emit this->buffersQueried(
                new BufferMeta{mmap(NULL, buf.length, PROT_READ | PROT_WRITE, this->fd, MAP_SHARED, buf.m.offset),
                               buf.length, this->fd});
    }
}