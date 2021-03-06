# OpenBB

> I will instruct thee and teach thee in the way which thou shalt go: I will guide thee with mine eye.

Psalm 32:8

This project is designed to provide an open source baby monitor solution. The key aims of this project are privacy, security, and of course, usability.

This repository will consist of three parts:

- A camera driver for all cameras supporting V4L2 and MJPEG that streams to a secondary server (C++/QT)
- A secondary server that converts to a web-friendly format and provides authorization, metrics logging, as well as static serving of web client (Spring/Java)
- A web client that presents the video in a cross-platform fashion (JavaScript/Angular)

The ultimate goal here is for a low-resource computer (say a raspberry PI) to be able to hook up to a Linux compatible webcam, and stream this data to a more sufficient server
(though with the split implementation, it should be fine to run them on the same machine and loop back if desired), and finally present a website that can be visited on any tablet,
phone, or PC within the local intranet.

This software is licensed under the MIT license (see LICENSE.txt) and provided in the hopes that it will be useful.

## Build Dependencies

Ubuntu:

> sudo apt-get install libqt5core5a libqt5websockets-dev cmake clang gdb 

NOTE: the Qt5_DIR cmake variable assumes that if you're in WSL, that it's running Ubuntu.