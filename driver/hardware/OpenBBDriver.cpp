//
// Created by grant_ogz8xax on 3/19/2020.
//
#include "OpenBBDriver.h"

OpenBBDriver::OpenBBDriver() {
    this->requester = new OpenBBRequester();
    this->marshaller = new OpenBBMarshaller();
    OpenBBDriver::connect(this->requester, &OpenBBRequester::buffersQueried, this->marshaller, &OpenBBMarshaller::seed);
    //in the future, this can configure the driver
    OpenBBDriver::connect(this, &OpenBBDriver::start, this->requester, &OpenBBRequester::configureBuffers);
    OpenBBDriver::connect(this, &OpenBBDriver::readyForMore, this->marshaller, &OpenBBMarshaller::stream);
    OpenBBDriver::connect(this, &OpenBBDriver::stop, this->marshaller, &OpenBBMarshaller::dequeueBuffers);
    OpenBBDriver::connect(this->marshaller, &OpenBBMarshaller::binaryReady, this, &OpenBBDriver::receiveBinary);
}

void OpenBBDriver::receiveBinary() {
    emit this->binaryReceived();
}