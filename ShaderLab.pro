TEMPLATE=subdirs

SUBDIRS = \
    qtpropertybrowser \
    shaderlabs

qtpropertybrowser.subdir = 3rd-party/qtpropertybrowser/buildlib
shaderlabs.subdir = shaderlabs

shaderlabs.depends = qtpropertybrowser
