cmake_minimum_required(VERSION 3.13)
project(OpenBB)

set(CMAKE_CXX_STANDARD 14)

add_executable(OpenBB driver/main.cpp driver/OpenBBDriver.cpp driver/OpenBBDriver.h driver/OpenBBMarshaller.cpp driver/OpenBBMarshaller.h driver/OpenBBRequester.cpp driver/OpenBBRequester.h driver/BufferMeta.cpp driver/BufferMeta.h)