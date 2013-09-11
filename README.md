draw-it
=======

This program is going to be able to recognize drawn Jump'n'Run levels on photos with OpenCV and convert them so that a simple Jump'n'Run game programmed in SFML will be able to use it.

How to build
============

This program is currently developed with the IDE QT-Creator and qmake, which uses a .pro file for build configuration. If you want to build the program with another IDE or maybe command line,
the following is required:
 - The OpenCV library and header files (I am using version 2.4.6)
 - The SFML library and header files (I am using version 2.1)
 - A c++11 compatible c++ compiler (I am using the g++ which is part of the gcc)
I havent't build the program without QT-Creator yet, so I can't give you any further information.

Building the Documentation
==========================

If you want to create the documentation yourself, which is included in the source code, you need the tool 'doxygen' (http://www.stack.nl/~dimitri/doxygen/).
A config file is alreay included in the repo. Just run doxygen like 'doxygen config' and there is a documentation in html and latex created.