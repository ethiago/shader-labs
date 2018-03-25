# ShaderLabs
![alt text](https://travis-ci.org/ethiago/shader-labs.svg?branch=master "Build")

ShaderLabs provides a abstraction over a OpenGL aplication so you can focus on learn or prototype your OpenGL vertex, fragment, geometry or tessellation shaders.

It is a Qt4 Gui application, with beautiful and easy to use interface.

![alt text](https://github.com/ethiago/shader-labs/raw/master/docs/img/screenshot1.png)

## Installation

For now, you can download a .deb package on the [release](https://github.com/ethiago/shader-labs/releases/latest) section of this repository or build the application from source.

For .deb package, on Ubuntu trusty (it's tested with [Travis CI](https://travis-ci.org/)), run

`$ sudo dpkg -i /path/to/file.deb`

this will let some broken references, so run

`$ sudo apt-get install -f`

to fix the issues and install the dependencies.

## Build from source

The project is intended to be multi platform, so it should be easy to build on Linux, macOS or Windows without problem. Let me know about any issue.

### Requirements

First you need to get Qt4. The installation of Qt is beyond the porpose of this guide, please, follow the the [installation guide](http://doc.qt.io/archives/qt-4.8/installation.html) of you plataform.

On Linux, it also needs libGLU, so install (libglu1-mesa).

### Build

1. Get the source `$ git clone https://github.com/ethiago/shader-labs.git ShaderLabs`
2. Navigate to source path `$ cd ShaderLabs`
3. Run the build script `$ ./build.sh`.

The binary will be located in ./build/app/bin

## Usage

This video has some basic usage of ShaderLabs and it a great starting point.

<a href="http://www.youtube.com/watch?feature=player_embedded&v=vApiyRTkFgI
" target="_blank"><img src="http://img.youtube.com/vi/vApiyRTkFgI/0.jpg"
alt="IMAGE ALT TEXT HERE" width="240" height="180" border="10" /></a>

If you want to learn more about shaders, this [tutorial](http://www.decom.ufop.br/sibgrapi2012/eproceedings/tutorials/t2-handouts.pdf) could help.
