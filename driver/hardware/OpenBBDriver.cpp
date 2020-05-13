//
// Created by grant_ogz8xax on 3/19/2020.
//
#include <fcntl.h>
#include "OpenBBDriver.h"
#include "BufferMeta.h";

OpenBBDriver::OpenBBDriver() {
    int fd = open("/dev/video0", O_RDWR);
    this->requester = new OpenBBRequester(fd);
}