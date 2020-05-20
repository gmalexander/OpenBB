//
// Created by grant_ogz8xax on 3/19/2020.
//

#include "OpenBBMarshaller.h"

OpenBBMarshaller::OpenBBMarshaller(QMessageLogger* log) {
    this->log = log;
    this->log->info("connecting marshaller lifecycle");
    OpenBBMarshaller::connect(this, &OpenBBMarshaller::seeded, this, &OpenBBMarshaller::queueBuffers, Qt::QueuedConnection);
    OpenBBMarshaller::connect(this, &OpenBBMarshaller::buffersQueued, this, &OpenBBMarshaller::stream, Qt::QueuedConnection);
    OpenBBMarshaller::connect(this, &OpenBBMarshaller::streaming, this, &OpenBBMarshaller::unstream, Qt::QueuedConnection);
    OpenBBMarshaller::connect(this, &OpenBBMarshaller::stoppedStreaming, this, &OpenBBMarshaller::createBinary, Qt::QueuedConnection);
}

void OpenBBMarshaller::seed(BufferMeta* bufferMeta) {
    this->log->info("received seed");
    this->meta = bufferMeta;
    this->log->info("reporting seed status");
    emit this->setDriverStatus(SEEDED);
    emit this->seeded();
}

void OpenBBMarshaller::queueBuffers() {
    this->log->info("queueing buffers of type video capture and memory map at index 0");
    v4l2_buffer buf;
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    buf.index = 0;
    int result = ioctl(this->meta->fd, VIDIOC_QBUF, &buf);
    if (result != 0) {
        this->log->critical("ioctl returned invalid value: %d", result);
        this->log->critical("queuing buffer failed for reason: %s", strerror(errno));
        emit this->fatalMarshallerError();
        return;
    }
    this->log->info("reporting buffers queued");
    emit this->setDriverStatus(QUEUING);
    emit this->buffersQueued();
}

void OpenBBMarshaller::stream() {
    this->log->info("beginning stream");
    int result = ioctl(this->meta->fd, VIDIOC_STREAMON);
    if (result != 0) {
        this->log->critical("ioctl returned invalid value: %d", result);
        this->log->critical("streaming failed for reason: %s", strerror(errno));
        emit this->fatalMarshallerError();
        return;
    }
    this->log->info("reporting streaming on");
    emit this->setDriverStatus(STREAMING);
    emit this->streaming();
}

void OpenBBMarshaller::unstream() {
    this->log->info("turning off stream");
    int result = ioctl(this->meta->fd, VIDIOC_STREAMOFF);
    if (result != 0) {
        this->log->critical("ioctl returned invalid value: %d", result);
        this->log->critical("turning streaming off failed for reason: %s", strerror(errno));
        emit this->fatalMarshallerError();
        return;
    }
    this->log->info("reporting that streaming has stopped");
    emit this->setDriverStatus(UNSTREAMING);
    emit this->stoppedStreaming();
}

void OpenBBMarshaller::dequeueBuffers() {
    this->log->info("dequeuing buffers of type video capture and memory map at index 0");
    v4l2_buffer buf;
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    buf.index = 0;
    int result = ioctl(this->meta->fd, VIDIOC_DQBUF, &buf);
    if (result == -1) {
        this->log->critical("ioctl returned invalid value: %d", result);
        this->log->critical("dequeuing buffer failed for reason: %s", strerror(errno));
        emit this->fatalMarshallerError();
        return;
    }
    this->log->info("reporting buffered dequeued");
    emit this->setDriverStatus(DEQUEUING);
    emit this->buffersDequeued();
}

void OpenBBMarshaller::createBinary() {
    this->log->info("creating binary of length %d", this->meta->length);
    char* start = static_cast<char*>(this->meta->start);
    this->log->info("creating and emitting binary");
    emit binaryReady(QByteArray(start, this->meta->length));
}