HEADERS       = pdfview.h
SOURCES       = pdfview.cpp main.cpp
INCLUDEPATH += ../lib /usr/include/poppler/qt4
LIBS += ../lib/libruncible.so -L/usr/lib -lpoppler-qt4 -lpoppler
TARGET=runcible-view-pdf
