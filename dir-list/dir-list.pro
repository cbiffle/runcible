include( ../common.pri )

HEADERS       = dirman.h
SOURCES       = dirman.cpp main.cpp
INCLUDEPATH += ../lib
LIBS += ../bin/libruncible.so

TARGET=runcible-dir-list
DESTDIR=../bin
