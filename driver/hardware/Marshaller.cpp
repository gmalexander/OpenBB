//
// Created by grant_ogz8xax on 3/19/2020.
//

#include "Marshaller.h"

namespace OpenBB {
    Marshaller::Marshaller(QMessageLogger *log) {
        this->log = log;
        this->log->info("connecting marshaller lifecycle");
        Marshaller::connect(this, &Marshaller::seeded, this, &Marshaller::queueBuffers,
                                  Qt::QueuedConnection);
        Marshaller::connect(this, &Marshaller::buffersQueued, this, &Marshaller::stream,
                                  Qt::QueuedConnection);
        Marshaller::connect(this, &Marshaller::streaming, this, &Marshaller::unstream,
                                  Qt::QueuedConnection);
        Marshaller::connect(this, &Marshaller::stoppedStreaming, this, &Marshaller::createBinary,
                                  Qt::QueuedConnection);
    }

    void Marshaller::seed(BufferMeta *bufferMeta) {
        emit this->setDriverStatus(SEEDING);
        this->log->info("received seed");
        this->meta = bufferMeta;
        this->log->info("reporting seed status");
        emit this->setDriverStatus(SEEDED);
        emit this->seeded();
    }

    void Marshaller::queueBuffers() {
        emit this->setDriverStatus(QUEUING);
        this->log->info("queueing buffers of type video capture and memory map at index 0");
        v4l2_buffer buf;
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = 0;
        int result = ioctl(this->meta->fd, VIDIOC_QBUF, &buf);
        if (result != 0) {
            this->log->critical("ioctl returned invalid value: %d", result);
            this->log->critical("queuing buffer failed for reason: %s", strerror(errno));
            emit this->sendError();
            return;
        }
        this->log->info("reporting buffers queued");
        emit this->setDriverStatus(QUEUED);
        emit this->buffersQueued();
    }

    void Marshaller::stream() {
        emit this->setDriverStatus(STREAM);
        this->log->info("beginning stream");
        int result = ioctl(this->meta->fd, VIDIOC_STREAMON);
        if (result != 0) {
            this->log->critical("ioctl returned invalid value: %d", result);
            this->log->critical("streaming failed for reason: %s", strerror(errno));
            emit this->sendError();
            return;
        }
        this->log->info("reporting streaming on");
        emit this->setDriverStatus(STREAMING);
        emit this->streaming();
    }

    void Marshaller::unstream() {
        emit this->setDriverStatus(UNSTREAM);
        this->log->info("turning off stream");
        int result = ioctl(this->meta->fd, VIDIOC_STREAMOFF);
        if (result != 0) {
            this->log->critical("ioctl returned invalid value: %d", result);
            this->log->critical("turning streaming off failed for reason: %s", strerror(errno));
            emit this->sendError();
            return;
        }
        this->log->info("reporting that streaming has stopped");
        emit this->setDriverStatus(UNSTREAMED);
        emit this->stoppedStreaming();
    }

    void Marshaller::dequeueBuffers() {
        emit this->setDriverStatus(DEQUEUING);
        this->log->info("dequeuing buffers of type video capture and memory map at index 0");
        v4l2_buffer buf;
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = 0;
        int result = ioctl(this->meta->fd, VIDIOC_DQBUF, &buf);
        if (result == -1) {
            this->log->critical("ioctl returned invalid value: %d", result);
            this->log->critical("dequeuing buffer failed for reason: %s", strerror(errno));
            emit this->sendError();
            return;
        }
        this->log->info("reporting buffered dequeued");
        emit this->setDriverStatus(DEQUEUED);
        emit this->buffersDequeued();
    }

    void Marshaller::createBinary() {
        this->log->info("creating binary of length %d", this->meta->length);
        char *start = static_cast<char *>(this->meta->start);
        this->log->info("creating and emitting binary");
        emit binaryReady(QByteArray(start, this->meta->length));
    }
}