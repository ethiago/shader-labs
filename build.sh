#!/bin/bash

rm -rf build
mkdir build
cd build
qmake ../ShaderLab.pro
make
cd ..