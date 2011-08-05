# -------------------------------------------------
# Project created by QtCreator 2011-03-26T13:25:06
# -------------------------------------------------
QT += core \
    gui \
    opengl
win32 { 
    TARGET = ShaderLab-win32
    INCLUDEPATH += lib/GLee5_4
    SOURCES += lib/GLee5_4/GLee.c
}
unix:TARGET = ShaderLab-unix32
TEMPLATE = app
DEPENDPATH += src
INCLUDEPATH += headers
OBJECTS_DIR = tmp
MOC_DIR = tmp
RCC_DIR = tmp
UI_DIR = tmp
DESTDIR = bin
SOURCES += src/main.cpp \
    src/Sphere.cpp \
    src/SLTabWidget.cpp \
    src/SLTabBar.cpp \
    src/Arcball.cpp \
    src/ChooseShaderDialog.cpp \
    src/SLCommandLinkButton.cpp \
    src/FileController.cpp \
    src/GLDisplay.cpp \
    src/Global.cpp \
    src/TextureController.cpp \
    src/Texture.cpp \
    src/TexturePropertiesView.cpp \
    src/Plane.cpp \
    src/MainController.cpp \
    src/MainWindow.cpp \
    src/Object3D.cpp \
    src/RenderController.cpp \
    src/ShaderCodeContainer.cpp \
    src/InterfaceRequests.cpp \
    src/ShaderController.cpp
HEADERS += headers/Sphere.h \
    headers/SLTabWidget.h \
    headers/SLTabBar.h \
    headers/Arcball.h \
    headers/ChooseShaderDialog.h \
    headers/SLCommandLinkButton.h \
    headers/FileController.h \
    headers/GLDisplay.h \
    headers/Global.h \
    headers/TextureController.h \
    headers/Texture.h \
    headers/TexturePropertiesView.h \
    headers/Plane.h \
    headers/MainController.h \
    headers/MainWindow.h \
    headers/Object3D.h \
    headers/RenderController.h \
    headers/ShaderCodeContainer.h \
    headers/InterfaceRequests.h \
    headers/ShaderController.h
FORMS += forms/MainWindow.ui \
    forms/ShaderCodeContainer.ui \
    forms/ChooseShaderDialog.ui \
    forms/TexturePropertiesView.ui
RESOURCES += shaderlab.qrc
OTHER_FILES += 
