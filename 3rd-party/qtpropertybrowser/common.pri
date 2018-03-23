infile(config.pri, SOLUTIONS_LIBRARY, yes): CONFIG += qtpropertybrowser-uselib

include(../../build.pri)

TEMPLATE += fakelib
QTPROPERTYBROWSER_LIBNAME = $$qtLibraryTarget(QtSolutions_PropertyBrowser-head)
TEMPLATE -= fakelib
QTPROPERTYBROWSER_LIBDIR = $$OUTPUT_APP_DIR/lib
unix:qtpropertybrowser-uselib:!qtpropertybrowser-buildlib:QMAKE_RPATHDIR += $$QTPROPERTYBROWSER_LIBDIR
