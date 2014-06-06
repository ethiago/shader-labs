#-------------------------------------------------
#
# Project created by QtCreator 2012-07-29T17:28:56
#
#-------------------------------------------------

CONFIG   += qtestlib
#CONFIG   -= app_bundle

TARGET = ShaderLabsTests

INCLUDEPATH += ../../headers
OBJECTS_DIR = tmp
MOC_DIR = tmp
RCC_DIR = tmp
UI_DIR = tmp
DESTDIR = bin

SOURCES += tst_projecttest.cpp \
    plytest.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"

HEADERS += plytest.h
