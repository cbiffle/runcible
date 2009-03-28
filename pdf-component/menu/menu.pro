SOURCES = main.cpp

LIBS += ../../bin/libruncible.so -L/usr/lib -lpoppler-qt4
INCLUDEPATH += ../../lib /usr/include/poppler/qt4

TARGET = runcible-open-ext-pdf
DESTDIR = ../../bin
