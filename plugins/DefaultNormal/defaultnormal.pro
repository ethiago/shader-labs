#-------------------------------------------------
#
# Project created by QtCreator 2014-06-14T17:00:23
#
#-------------------------------------------------

TEMPLATE        = lib
CONFIG         += plugin

HESRC=../../hedsmodule/src
HEHED=../../hedsmodule/headers

INCLUDEPATH    += ../../pluginsInterfaces $$HEHED

HEADERS += defaultnormal.h \
       $$HEHED/vertex.h \
       $$HEHED/face.h \
       $$HEHED/halfedge.h

SOURCES += defaultnormal.cpp \
       $$HESRC/vertex.cpp \
       $$HESRC/face.cpp \
       $$HESRC/halfedge.cpp

TARGET          = $$qtLibraryTarget(defaultnormalplugin)
DESTDIR         = ../../../ShaderLabs/plugins
