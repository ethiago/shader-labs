#!/bin/bash

APPPATH="build"
DEBPATH="$APPPATH/DEBIAN"
BINPATH=usr/$BINNAME/$BINNAME-$VER

mkdir $DEBPATH

cat > "$DEBPATH/control" << EOM
Source: ShaderLabs
Package: ShaderLabs
Version: $VER
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

cat > "$DEBPATH/postinst" << EOM
ln -sf /$BINPATH/bin/$BINNAME /usr/bin/$BINNAME
EOM

chmod +x $DEBPATH/postinst

mkdir -p $APPPATH/$BINPATH

mv $APPPATH/bin $APPPATH/$BINPATH/
mv $APPPATH/lib $APPPATH/$BINPATH/

dpkg-deb --build $APPPATH
EXC=$?

mv ./build/app.deb ./$BINNAME\_$VER.deb

exit $EXC
