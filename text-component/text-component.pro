include( ../common.pri )

HEADERS       = pageview.h
SOURCES       = main.cpp pageview.cpp
INCLUDEPATH += ../lib
LIBS += ../bin/libruncible.so

TARGET=runcible-open-ext-txt
DESTDIR=../bin
