include( ../../common.pri )

SOURCES = main.cpp

LIBS += ../../bin/libruncible.so -L/usr/lib
INCLUDEPATH += ../../lib

TARGET = runcible-open-ext-pdf
DESTDIR = ../../bin
