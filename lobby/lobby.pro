include( ../common.pri )

HEADERS       = footer.h coord-server.h keyguard.h batmon.h keypadim.h
SOURCES       = main.cpp footer.cpp coord-server.cpp keyguard.cpp batmon.cpp keypadim.cpp
INCLUDEPATH += ../lib
LIBS += ../bin/libruncible.so

TARGET=runcible-lobby
DESTDIR=../bin
