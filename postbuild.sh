#!/bin/bash

APPPATH="buuild/app"
DEBPATH="$APPPATH/DEBIAN"

mkdir $DEBPATH

version="$1"

read -d '' control <<- EOF
Source: ShaderLabs
Package: ShaderLabs
Maintainer: Thiago Gomes <thiagoegomes@gmail.com>
Section: graphics
Priority: optional
Architecture: all
Depends: libqt4-core, libqt4-gui, libqt4-xml, libqt4-opengl
Description: ShaderLabs is a IDE for GLSL experimentation
 ShaderLabs provides a abstraction over a OpenGL aplication so you can 
 focus on learn or propotype your OpenGL vertex, fragment, geometry 
 or tessellation shaders.
EOF 

echo "Version: $version\n$control" >> $DEBPATH/control

read -d '' post <<- EOF
echo "Instalation success"
EOF

echo "$post" >> $DEBPATH/postinst
chmod +x $DEBPATH/postinst

dpkg-deb --build $APPPATH
