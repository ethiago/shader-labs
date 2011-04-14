#-------------------------------------------------
#
# Project created by QtCreator 2011-04-09T20:46:21
#
#-------------------------------------------------

QT       += core opengl testlib gui

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

OBJECTS += ../../../ShaderLab-build-desktop/tmp/mainwindow.o
OBJECTS += ../../../ShaderLab-build-desktop/tmp/moc_mainwindow.o

OBJECTS += ../../../ShaderLab-build-desktop/tmp/codechoicedialog.o
OBJECTS += ../../../ShaderLab-build-desktop/tmp/moc_codechoicedialog.o

OBJECTS += ../../../ShaderLab-build-desktop/tmp/shadercodecontainer.o
OBJECTS += ../../../ShaderLab-build-desktop/tmp/moc_shadercodecontainer.o

OBJECTS += ../../../ShaderLab-build-desktop/tmp/commandlinkbutton.o
OBJECTS += ../../../ShaderLab-build-desktop/tmp/moc_commandlinkbutton.o

OBJECTS += ../../../ShaderLab-build-desktop/tmp/gldisplay.o
OBJECTS += ../../../ShaderLab-build-desktop/tmp/moc_gldisplay.o

OBJECTS += ../../../ShaderLab-build-desktop/tmp/global.o

SOURCES += tst_filecontrollerteste.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"

RESOURCES += \
    ShaderLabUnitTestes.qrc
