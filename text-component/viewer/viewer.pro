include( ../../common.pri )

HEADERS       = pageview.h
SOURCES       = main.cpp pageview.cpp
INCLUDEPATH += ../../lib
LIBS += -L../../bin -lruncible

TARGET=runcible-open-ext-txt
DESTDIR=../../bin
