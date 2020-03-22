//
// Created by grant_ogz8xax on 3/19/2020.
//

#ifndef OPENBB_OPENBBMARSHALLER_H
#define OPENBB_OPENBBMARSHALLER_H
#include "BufferMeta.h"
#include <vector>
#include <linux/videodev2.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <cerrno>
#include <cstring>
#include <iostream>

class OpenBBMarshaller {
public:
    explicit OpenBBMarshaller(BufferMeta meta, int fd): meta{meta}, fd{fd} {}
    void queueBuffers();
    void dequeueBuffers();
    void stream();
    void unstream();
    std::vector<char> getBinary();
private:
    BufferMeta meta;
    int fd;
};


#endif //OPENBB_OPENBBMARSHALLER_H
