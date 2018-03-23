#!/bin/bash

APPPATH="build/app"
DEBPATH="$APPPATH/DEBIAN"

mkdir $DEBPATH

VERSION=$1

cat > "$DEBPATH/control" << EOM
Source: ShaderLabs
Package: ShaderLabs
Version: $VERSION
Maintainer: Thiago Gomes <thiagoegomes@gmail.com>
Section: graphics
Priority: optional
Architecture: all
Depends: libqt4-core, libqt4-gui, libqt4-xml, libqt4-opengl
Description: ShaderLabs is a IDE for GLSL experimentation
 ShaderLabs provides a abstraction over a OpenGL aplication so you can 
 focus on learn or propotype your OpenGL vertex, fragment, geometry 
 or tessellation shaders.
EOM

cat > "$DEBPATH/postinst" << EOM
echo "Instalation success"
EOM

chmod +x $DEBPATH/postinst

dpkg-deb --build $APPPATH
