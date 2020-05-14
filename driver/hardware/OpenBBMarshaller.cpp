//
// Created by grant_ogz8xax on 3/19/2020.
//

#include "OpenBBMarshaller.h"

void OpenBBMarshaller::seed(BufferMeta* bufferMeta) {
    this->meta = bufferMeta;
}

void OpenBBMarshaller::queueBuffers() {
    v4l2_buffer buf;
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    buf.index = 0;
    int result = ioctl(this->meta->fd, VIDIOC_QBUF, &buf);
    if (result != 0) {
       std::cout << "Queuing buffer failed for reason: " << strerror(errno) << "\n";
    }
    emit this->buffersQueued();
}

void OpenBBMarshaller::stream() {
    int result = ioctl(this->meta->fd, VIDIOC_STREAMON);
    if (result != 0) {
        std::cout << "Streaming failed for reason: " << strerror(errno) << "\n";
    }
    emit this->streaming();
}

void OpenBBMarshaller::unstream() {
    int result = ioctl(this->meta->fd, VIDIOC_STREAMOFF);
    if (result != 0) {
        std::cout << "Turning streaming off failed for reason: " << strerror(errno) << "\n";
    }
    emit this->stoppedStreaming();
}

void OpenBBMarshaller::dequeueBuffers() {
    v4l2_buffer buf;
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    buf.index = 0;
    int result = ioctl(this->meta->fd, VIDIOC_DQBUF, &buf);
    if (result == -1) {
        std::cout << "Dequeuing buffer failed for reason: " << strerror(errno) << "\n";
    }
    emit this->buffersDequeued();
}

void OpenBBMarshaller::createBinary() {
    char* start = static_cast<char*>(this->meta->start);
    emit binaryReady(std::vector<char>(start, start+this->meta->length));
}