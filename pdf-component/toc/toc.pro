HEADERS = tocman.h
SOURCES = main.cpp tocman.cpp

LIBS += ../../bin/libruncible.so -L/usr/lib -lpoppler-qt4
INCLUDEPATH += ../../lib /usr/include/poppler/qt4

TARGET = runcible-contents-pdf
DESTDIR = ../../bin
