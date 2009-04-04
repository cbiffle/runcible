include( ../common.pri )

HEADERS       = footer.h coord-server.h keyguard.h
SOURCES       = main.cpp footer.cpp coord-server.cpp keyguard.cpp
INCLUDEPATH += ../lib
LIBS += ../bin/libruncible.so

TARGET=runcible-lobby
DESTDIR=../bin
