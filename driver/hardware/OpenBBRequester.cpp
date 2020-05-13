//
// Created by grant_ogz8xax on 3/19/2020.
//

#include "OpenBBRequester.h"

void OpenBBRequester::configureBuffers() {
    v4l2_format format;
    format.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG;
    format.fmt.pix.height = 800;
    format.fmt.pix.width = 600;
    int result = ioctl(this->fd, VIDIOC_S_FMT, &format);
    if (result != 0) {
        std::cout << "Setting format failed for reason: " << strerror(errno) << "\n";
    }
}

void OpenBBRequester::requestBuffers() {
    v4l2_requestbuffers reqbuf;
    reqbuf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    reqbuf.count = 1;
    reqbuf.memory = V4L2_MEMORY_MMAP;
    int result = ioctl(this->fd, VIDIOC_REQBUFS, &reqbuf);
    if (result != 0) {
        std::cout << "Requesting buffers failed for reason: " << strerror(errno) << "\n";
    }
}

OpenBBMarshaller* OpenBBRequester::queryBuffers() {
    v4l2_buffer buf;
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.index = 0;
    int result = ioctl(this->fd, VIDIOC_QUERYBUF, &buf);
    if (result != 0) {
        std::cout << "Querying buffers failed for reason: " << strerror(errno) << "\n";
    }
    BufferMeta meta{mmap(NULL, buf.length, PROT_READ | PROT_WRITE, this->fd, MAP_SHARED, buf.m.offset), buf.length};
    return new OpenBBMarshaller(meta, this->fd);
}