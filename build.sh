#!/usr/bin/env bash

rm -rf build
mkdir build
cd build
qmake ../ShaderLabs.pro && make
EXC=$?
cd ..

exit $EXC
