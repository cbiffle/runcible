include( ../common.pri )

HEADERS       =
SOURCES       = main.cpp
INCLUDEPATH += ../lib
LIBS += ../bin/libruncible.so

TARGET=runcible-lobby
DESTDIR=../bin
