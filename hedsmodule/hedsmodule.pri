HESRC=$$PWD/src
HEHED=$$PWD/headers

DEPENDPATH  += $$HESRC
INCLUDEPATH += $$HEHED

SOURCES += \
    $$HESRC/vertex.cpp \
    $$HESRC/face.cpp \
    $$HESRC/halfedge.cpp \
    $$HESRC/halfedgedata.cpp

HEADERS += \
    $$HEHED/vertex.h \
    $$HEHED/face.h \
    $$HEHED/halfedge.h \
    $$HEHED/halfedgedata.h
