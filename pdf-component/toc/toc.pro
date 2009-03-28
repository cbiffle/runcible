CONFIG += link_pkgconfig
HEADERS = tocman.h
SOURCES = main.cpp tocman.cpp

PKGCONFIG += poppler-qt4
INCLUDEPATH += ../../lib
LIBS += ../../bin/libruncible.so

TARGET = runcible-contents-pdf
DESTDIR = ../../bin
