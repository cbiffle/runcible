include( ../../common.pri )

HEADERS       = 
SOURCES       = main.cpp
INCLUDEPATH += ../lib
INCLUDEPATH += /usr/local/lib # CLucene puts a header there, for some reason.
LIBS += ../../bin/libruncible.so ../../bin/libruncible-search.so -lclucene -lprotobuf

TARGET=runcible-search-index
DESTDIR=../../bin
