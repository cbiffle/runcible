CONFIG += link_pkgconfig
HEADERS = pdfview.h 
SOURCES = pdfview.cpp main.cpp

PKGCONFIG += poppler-qt4
LIBS += ../../bin/libruncible.so

TARGET = runcible-view-pdf
DESTDIR = ../../bin
