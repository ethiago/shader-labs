# -------------------------------------------------
# Project created by QtCreator 2011-03-26T13:25:06
# -------------------------------------------------
QT += core \
    gui \
    opengl \
    xml
win32 { 
    TARGET = ../ShaderLab-win32
    LIBS += -lfreeglut -lopengl32
}

unix {
    TARGET = ShaderLab-unix32
    #LIBS += -lGLU -ldl    #does't work on Mac
}
#MAC
LIBS += -framework Carbon
#/MAC

TEMPLATE = app
DEPENDPATH += ../src
INCLUDEPATH += ../headers
OBJECTS_DIR = tmp
MOC_DIR = tmp
RCC_DIR = tmp
UI_DIR = tmp
DEPLOYDIR = ../../ShaderLabs
DESTDIR = $$DEPLOYDIR/bin

#QtSolutions_PropertyBrowser-head
include(../3rd-party/qtpropertybrowser/src/qtpropertybrowser.pri)
#QtSolutions_PropertyBrowser-head

SLSRC=../src
SLHED=../headers
SLFOR=../forms
SLRES=../qresources

SOURCES += $$SLSRC/gl3w.c \
    $$SLSRC/SLShader2.cpp \
    $$SLSRC/SLShaderProgram.cpp \
    $$SLSRC/main.cpp \
    $$SLSRC/Sphere.cpp \
    $$SLSRC/SLTabWidget.cpp \
    $$SLSRC/SLTabBar.cpp \
    $$SLSRC/Arcball.cpp \
    $$SLSRC/ChooseShaderDialog.cpp \
    $$SLSRC/SLCommandLinkButton.cpp \
    $$SLSRC/GLDisplay.cpp \
    $$SLSRC/Global.cpp \
    $$SLSRC/Texture.cpp \
    $$SLSRC/TexturePropertiesView.cpp \
    $$SLSRC/Plane.cpp \
    $$SLSRC/MainController.cpp \
    $$SLSRC/MainWindow.cpp \
    $$SLSRC/Object3D.cpp \
    $$SLSRC/RenderController.cpp \
    $$SLSRC/InterfaceRequests.cpp \
    $$SLSRC/Find.cpp \
    $$SLSRC/HighLighter.cpp \
    $$SLSRC/PrimitivesDialog.cpp \
    $$SLSRC/DirectionalLight.cpp \
    $$SLSRC/Project.cpp \
    $$SLSRC/Cube.cpp \
    $$SLSRC/GlobalProperties.cpp \
    $$SLSRC/Tetrahedron.cpp \
    $$SLSRC/Point.cpp \
    $$SLSRC/Scene3D.cpp \
    $$SLSRC/SLFile.cpp \
    $$SLSRC/SLTextures.cpp \
    $$SLSRC/EditorController.cpp \
    $$SLSRC/SLCodeContainer.cpp \
    $$SLSRC/codeeditor.cpp \
    $$SLSRC/Texture3D.cpp \
    $$SLSRC/SLTexture3dDialog.cpp \
    $$SLSRC/TrianglePatch.cpp \
    $$SLSRC/IcosahedronPatch.cpp \
    $$SLSRC/Patch.cpp \
    $$SLSRC/SLShaderProgramController.cpp \
    $$SLSRC/SLObject2.cpp \
    $$SLSRC/SLObjectController.cpp \
    $$SLSRC/slgl3w.cpp \
    ../src/squarepatch.cpp

HEADERS += $$SLHED/gl3w.h \
    $$SLHED/glcorearb.h \
    $$SLHED/Sphere.h \
    $$SLHED/SLTabWidget.h \
    $$SLHED/SLTabBar.h \
    $$SLHED/Arcball.h \
    $$SLHED/ChooseShaderDialog.h \
    $$SLHED/SLCommandLinkButton.h \
    $$SLHED/GLDisplay.h \
    $$SLHED/Global.h \
    $$SLHED/Texture.h \
    $$SLHED/TexturePropertiesView.h \
    $$SLHED/Plane.h \
    $$SLHED/MainController.h \
    $$SLHED/MainWindow.h \
    $$SLHED/Object3D.h \
    $$SLHED/RenderController.h \
    $$SLHED/InterfaceRequests.h \
    $$SLHED/Find.h \
    $$SLHED/HighLighter.h \
    $$SLHED/PrimitivesDialog.h \
    $$SLHED/DirectionalLight.h \
    $$SLHED/Project.h \
    $$SLHED/Cube.h \
    $$SLHED/GlobalProperties.h \
    $$SLHED/Tetrahedron.h \
    $$SLHED/Point.h \
    $$SLHED/Scene3D.h \
    $$SLHED/SLFile.h \
    $$SLHED/SLTextures.h \
    $$SLHED/EditorController.h \
    $$SLHED/SLCodeContainer.h \
    $$SLHED/codeeditor.h \
    $$SLHED/Texture3D.h \
    $$SLHED/SLTexture3dDialog.h \
    $$SLHED/TrianglePatch.h \
    $$SLHED/IcosahedronPatch.h \
    $$SLHED/Patch.h \
    $$SLHED/SLShaderProgram.h \
    $$SLHED/SLShader2.h \
    $$SLHED/SLShaderProgramController.h \
    $$SLHED/SLObject2.h \
    $$SLHED/SLObjectController.h \
    $$SLHED/slgl3w.h \
    ../headers/squarepatch.h


FORMS += $$SLFOR/MainWindow.ui \
    $$SLFOR/ShaderCodeContainer.ui \
    $$SLFOR/ChooseShaderDialog.ui \
    $$SLFOR/TexturePropertiesView.ui \
    $$SLFOR/Find.ui \
    $$SLFOR/PrimitivesDialog.ui \
    $$SLFOR/GlobalProperties.ui \
    $$SLFOR/SLTexture3dDialog.ui
    
RESOURCES += $$SLRES/shaderlab.qrc
OTHER_FILES += 
