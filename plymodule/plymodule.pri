PLYSRC=$$PWD/src
PLYHED=$$PWD/headers

DEPENDPATH  += $$PLYSRC
INCLUDEPATH += $$PLYHED

SOURCES += \
    $$PLYSRC/meshextraction.cpp \
    $$PLYSRC/plydata.cpp \
    $$PLYSRC/plydataitens.cpp \
    $$PLYSRC/plyio.cpp

HEADERS += \
    $$PLYHED/meshcontainer.h \
    $$PLYHED/meshextraction.h \
    $$PLYHED/plydata.h \
    $$PLYHED/plydataitens.h \
    $$PLYHED/plyio.h
