#!/bin/bash

APPPATH="build/app"
DEBPATH="$APPPATH/DEBIAN"
BINNAME="shaderlabs"
VERSION=$1
BINPATH=usr/shaderlabs/shaderlabs-$VERSION/

rm -rf $DEBPATH
mkdir $DEBPATH

cat > "$DEBPATH/control" << EOM
Source: ShaderLabs
Package: ShaderLabs
Version: $VERSION
Maintainer: Thiago Gomes <thiagoegomes@gmail.com>
Section: graphics
Priority: optional
Architecture: all
Depends: libqt4-core, libqt4-gui, libqt4-xml, libqt4-opengl, libglu1-mesa
Description: ShaderLabs is a IDE for GLSL experimentation
 ShaderLabs provides a abstraction over a OpenGL aplication so you can 
 focus on learn or propotype your OpenGL vertex, fragment, geometry 
 or tessellation shaders.
EOM
