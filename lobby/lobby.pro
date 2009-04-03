include( ../common.pri )

HEADERS       =  footer.h
SOURCES       = main.cpp footer.cpp
INCLUDEPATH += ../lib
LIBS += ../bin/libruncible.so

TARGET=runcible-lobby
DESTDIR=../bin
