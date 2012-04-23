# -------------------------------------------------
# Project created by QtCreator 2011-03-26T13:25:06
# -------------------------------------------------
QT += core \
    gui \
    opengl \
    xml
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

include(../qt-solutions/qtpropertybrowser/src/qtpropertybrowser.pri)

SOURCES += src/main.cpp \
    src/Sphere.cpp \
    src/SLTabWidget.cpp \
    src/SLTabBar.cpp \
    src/Arcball.cpp \
    src/ChooseShaderDialog.cpp \
    src/SLCommandLinkButton.cpp \
    src/GLDisplay.cpp \
    src/Global.cpp \
    src/Texture.cpp \
    src/TexturePropertiesView.cpp \
    src/Plane.cpp \
    src/MainController.cpp \
    src/MainWindow.cpp \
    src/Object3D.cpp \
    src/RenderController.cpp \
    src/InterfaceRequests.cpp \
    src/Find.cpp \
    src/HighLighter.cpp \
    src/PrimitivesDialog.cpp \
    src/DirectionalLight.cpp \
    src/Project.cpp \
    src/Cube.cpp \
    src/GlobalProperties.cpp \
    src/Tetrahedron.cpp \
    src/Point.cpp \
    src/Scene3D.cpp \
    src/SLObject.cpp \
    src/SLShader.cpp \
    src/SLFile.cpp \
    src/SLTextures.cpp \
    src/EditorController.cpp \
    src/SLCodeContainer.cpp \
    src/codeeditor.cpp \
    src/Texture3D.cpp \
    src/SLTexture3dDialog.cpp
HEADERS += headers/Sphere.h \
    headers/SLTabWidget.h \
    headers/SLTabBar.h \
    headers/Arcball.h \
    headers/ChooseShaderDialog.h \
    headers/SLCommandLinkButton.h \
    headers/GLDisplay.h \
    headers/Global.h \
    headers/Texture.h \
    headers/TexturePropertiesView.h \
    headers/Plane.h \
    headers/MainController.h \
    headers/MainWindow.h \
    headers/Object3D.h \
    headers/RenderController.h \
    headers/InterfaceRequests.h \
    headers/Find.h \
    headers/HighLighter.h \
    headers/PrimitivesDialog.h \
    headers/DirectionalLight.h \
    headers/Project.h \
    headers/Cube.h \
    headers/GlobalProperties.h \
    headers/Tetrahedron.h \
    headers/Point.h \
    headers/Scene3D.h \
    headers/SLObject.h \
    headers/SLShader.h \
    headers/SLFile.h \
    headers/SLTextures.h \
    headers/EditorController.h \
    headers/SLCodeContainer.h \
    headers/codeeditor.h \
    headers/Texture3D.h \
    headers/SLTexture3dDialog.h
FORMS += forms/MainWindow.ui \
    forms/ShaderCodeContainer.ui \
    forms/ChooseShaderDialog.ui \
    forms/TexturePropertiesView.ui \
    forms/Find.ui \
    forms/PrimitivesDialog.ui \
    forms/GlobalProperties.ui \
    forms/SLTexture3dDialog.ui
RESOURCES += shaderlab.qrc
OTHER_FILES += 
