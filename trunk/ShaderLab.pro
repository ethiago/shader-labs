# -------------------------------------------------
# Project created by QtCreator 2011-03-26T13:25:06
# -------------------------------------------------
QT += core \
    gui \
    opengl
TARGET = ShaderLab
TEMPLATE = app
DEPENDPATH += src
INCLUDEPATH += headers
OBJECTS_DIR = tmp
MOC_DIR = tmp
RCC_DIR = tmp
UI_DIR = tmp
DESTDIR = bin
SOURCES += src/main.cpp \
    src/mainwindow.cpp \
    src/codechoicedialog.cpp \
    src/filecontroller.cpp \
    src/gldisplay.cpp \
    src/maincontroller.cpp \
    src/global.cpp \
    src/shadercodecontainer.cpp \
    src/commandlinkbutton.cpp \
    src/object3d.cpp \
    src/sphere.cpp \
    src/rendercontroller.cpp \
    src/arcball.cpp
HEADERS += headers/mainwindow.h \
    headers/codechoicedialog.h \
    headers/global.h \
    headers/filecontroller.h \
    headers/gldisplay.h \
    headers/maincontroller.h \
    headers/shadercodecontainer.h \
    headers/commandlinkbutton.h \
    headers/object3d.h \
    headers/sphere.h \
    headers/rendercontroller.h \
    headers/arcball.h
FORMS += forms/mainwindow.ui \
    forms/codechoicedialog.ui \
    forms/shadercodecontainer.ui
RESOURCES += shaderlab.qrc
