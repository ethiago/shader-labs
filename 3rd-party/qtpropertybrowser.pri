QTPB = $$PWD/qt-solutions/qtpropertybrowser/src

INCLUDEPATH += $$QTPB
DEPENDPATH += $$QTPB

SOURCES += $$QTPB/qtpropertybrowser.cpp \
        $$QTPB/qtpropertymanager.cpp \
        $$QTPB/qteditorfactory.cpp \
        $$QTPB/qtvariantproperty.cpp \
        $$QTPB/qttreepropertybrowser.cpp \
        $$QTPB/qtbuttonpropertybrowser.cpp \
        $$QTPB/qtgroupboxpropertybrowser.cpp \
        $$QTPB/qtpropertybrowserutils.cpp

HEADERS += $$QTPB/qtpropertybrowser.h \
        $$QTPB/qtpropertymanager.h \
        $$QTPB/qteditorfactory.h \
        $$QTPB/qtvariantproperty.h \
        $$QTPB/qttreepropertybrowser.h \
        $$QTPB/qtbuttonpropertybrowser.h \
        $$QTPB/qtgroupboxpropertybrowser.h \
        $$QTPB/qtpropertybrowserutils_p.h

RESOURCES += $$QTPB/qtpropertybrowser.qrc
