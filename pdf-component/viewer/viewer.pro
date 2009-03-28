HEADERS = pdfview.h 
SOURCES = pdfview.cpp main.cpp

LIBS += ../../bin/libruncible.so -L/usr/lib -lpoppler-qt4
INCLUDEPATH += /usr/include/poppler/qt4

TARGET = runcible-view-pdf
DESTDIR = ../../bin
