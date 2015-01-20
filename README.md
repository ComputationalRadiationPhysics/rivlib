# RivLib

## Install (Release Mode)

```bash
# sudo apt-get install zlib1g-dev uuid-dev cmake  # Release
# sudo apt-get install libxmu-dev libxi-dev freeglut3-dev  # Debug/Tests
#
# for MJPEG support (libjpeg-turbo from http://libjpeg-turbo.virtualgl.org )
# sudo apt-get install nasm
#   download & install e.g. v1.3.1 from
#   http://sourceforge.net/projects/libjpeg-turbo/files/
# export CMAKE_PREFIX_PATH=<PATH_TO>/libjpeg-turbo-1.3.1:$CMAKE_PREFIX_PATH
# export LD_LIBRARY_PATH=<PATH_TO>/libjpeg-turbo-1.3.1/lib:$LD_LIBRARY_PATH

cd libs
./build.sh <INSTALL_PREFIX>
cd rivlib/release
[sudo] make install
```

## Known Issues

On some systems, you might have to export
```bash
export CMAKE_PREFIX_PATH=/usr/lib/x86_64-linux-gnu/
```
so `<uuid/uuid.h>` and other shared libs can be found.

## Set Env

```bash
export RIVLIB_ROOT=<INSTALL_PREFIX>
export VISLIB_ROOT=$RIVLIB_ROOT
export THELIB_ROOT=$RIVLIB_ROOT
```

## Licenses

This repository contains `rivlib` and snapshots of the libraries
[thelib++](http://thelib.sourceforge.net/index.php/TheLib) and
[vislib](https://svn.vis.uni-stuttgart.de/trac/vislib).
The according licenses are listed in `libs/<name>`.
