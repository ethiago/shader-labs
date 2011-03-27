#-------------------------------------------------
#
# Project created by QtCreator 2011-03-26T13:25:06
#
#-------------------------------------------------

QT       += core gui

TARGET = ShaderLab
TEMPLATE = app

DEPENDPATH += src
INCLUDEPATH += headers
OBJECTS_DIR = tmp
MOC_DIR = tmp
RCC_DIR = tmp
UI_DIR = tmp
DESTDIR = bin

SOURCES += src/main.cpp\
        src/mainwindow.cpp \
    src/codechoicedialog.cpp \
    src/filecontroller.cpp

HEADERS  += headers/mainwindow.h \
    headers/codechoicedialog.h \
    headers/global.h \
    headers/filecontroller.h

FORMS    += forms/mainwindow.ui \
        forms/codechoicedialog.ui

RESOURCES += \
    shaderlab.qrc
