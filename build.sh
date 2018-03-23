#!/usr/bin/env bash

rm -rf build
mkdir build
cd build
qmake ../ShaderLab.pro && make
EXC=$?
cd ..

exit $EXC