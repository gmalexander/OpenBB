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
add_executable(OpenBB driver/hardware/Marshaller.cpp driver/hardware/Marshaller.h driver/hardware/Requester.cpp driver/hardware/Requester.h driver/hardware/BufferMeta.h driver/DriverServer.cpp driver/DriverServer.h driver/WebSocket.cpp driver/WebSocket.h driver/hardware/DriverStatus.h driver/hardware/ErrorOrigin.h)

target_link_libraries(OpenBB Qt5::WebSockets Qt5::Core)