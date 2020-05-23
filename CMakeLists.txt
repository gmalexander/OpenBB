cmake_minimum_required(VERSION 3.0)
project(OpenBB)

message("Building for ${CMAKE_SYSTEM_NAME} version ${CMAKE_SYSTEM_VERSION}")

if(${CMAKE_SYSTEM_VERSION} MATCHES "Mic")
    set(Qt5_DIR "/usr/lib/x86_64-linux-gnu/cmake/Qt5")
endif()

set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 14)

find_package(Qt5 COMPONENTS WebSockets Core REQUIRED)
add_executable(OpenBB driver/hardware/OpenBBMarshaller.cpp driver/hardware/OpenBBMarshaller.h driver/hardware/OpenBBRequester.cpp driver/hardware/OpenBBRequester.h driver/hardware/BufferMeta.h driver/OpenBBDriverServer.cpp driver/OpenBBDriverServer.h driver/OpenBBWebSocket.cpp driver/OpenBBWebSocket.h driver/hardware/DriverStatus.h)

target_link_libraries(OpenBB Qt5::WebSockets Qt5::Core)