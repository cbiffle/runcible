include( ../common.pri )

HEADERS       = footer.h coord-server.h
SOURCES       = main.cpp footer.cpp coord-server.cpp
INCLUDEPATH += ../lib
LIBS += ../bin/libruncible.so

TARGET=runcible-lobby
DESTDIR=../bin
