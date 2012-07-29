#-------------------------------------------------
#
# Project created by QtCreator 2012-07-29T17:28:56
#
#-------------------------------------------------

QT       += opengl xml testlib

TARGET = tst_projecttest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

unix:TARGET = ShaderLabTest
INCLUDEPATH += ../../headers
OBJECTS_DIR = tmp
MOC_DIR = tmp
RCC_DIR = tmp
UI_DIR = tmp
DESTDIR = bin

SOURCES += tst_projecttest.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"
