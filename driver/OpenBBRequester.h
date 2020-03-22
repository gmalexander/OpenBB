//
// Created by grant_ogz8xax on 3/19/2020.
//

#ifndef OPENBB_OPENBBREQUESTER_H
#define OPENBB_OPENBBREQUESTER_H
#include <vector>
#include <iostream>
#include <linux/videodev2.h>
#include <sys/ioctl.h>
#include <cstring>
#include <sys/mman.h>
#include <fcntl.h>
#include "BufferMeta.h"

class OpenBBRequester {
public:
    explicit OpenBBRequester(int fd): fd{fd} {}
    void configureBuffers();
    void requestBuffers();
    BufferMeta queryBuffers();
private:
    int fd;
};


#endif //OPENBB_OPENBBREQUESTER_H
