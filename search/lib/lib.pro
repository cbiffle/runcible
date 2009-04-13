include( ../../common.pri )

TEMPLATE = lib
HEADERS = proto/indexer.pb.h
SOURCES = proto/indexer.pb.cc
LIBS += -lprotobuf

TARGET=runcible-search
DESTDIR=../../bin
