//
// Created by grant_ogz8xax on 3/19/2020.
//

#include "OpenBBMarshaller.h"

void OpenBBMarshaller::queueBuffers() {
    v4l2_buffer buf;
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    buf.index = 0;
    int result = ioctl(this->fd, VIDIOC_QBUF, &buf);
    if (result != 0) {
       std::cout << "Queuing buffer failed for reason: " << strerror(errno) << "\n";
    }
}

void OpenBBMarshaller::stream() {
    int result = ioctl(this->fd, VIDIOC_STREAMON);
    if (result != 0) {
        std::cout << "Streaming failed for reason: " << strerror(errno) << "\n";
    }
}

void OpenBBMarshaller::unstream() {
    int result = ioctl(this->fd, VIDIOC_STREAMOFF);
    if (result != 0) {
        std::cout << "Turning streaming off failed for reason: " << strerror(errno) << "\n";
    }
}

std::vector<char> OpenBBMarshaller::getBinary() {
    char* start = static_cast<char*>(this->meta.start);
    return std::vector<char>(start, start+this->meta.length);
}