include( ../../common.pri )

QT += xml
CONFIG += link_pkgconfig
HEADERS = pdfview.h 
SOURCES = pdfview.cpp main.cpp

PKGCONFIG += poppler poppler-qt4
LIBS += ../../bin/libruncible.so
INCLUDEPATH += ../../lib

TARGET = runcible-view-pdf
DESTDIR = ../../bin
