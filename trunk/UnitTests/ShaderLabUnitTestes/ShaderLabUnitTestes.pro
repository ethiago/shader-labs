#-------------------------------------------------
#
# Project created by QtCreator 2011-04-09T20:46:21
#
#-------------------------------------------------

QT       += opengl testlib

TARGET = tst_filecontrollerteste
CONFIG   += console
CONFIG   -= app_bundle
OBJECTS_DIR = tmp
MOC_DIR = tmp
RCC_DIR = tmp
UI_DIR = tmp
DESTDIR = bin
TEMPLATE = app

OBJECTS += ../../../ShaderLab-build-desktop/tmp/filecontroller.o
OBJECTS += ../../../ShaderLab-build-desktop/tmp/moc_filecontroller.o

SOURCES += tst_filecontrollerteste.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"

RESOURCES += \
    ShaderLabUnitTestes.qrc
