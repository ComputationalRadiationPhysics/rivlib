# RivLib

## Install (Release Mode)

```bash
# sudo apt-get install zlib1g-dev uuid-dev cmake  # Release
# sudo apt-get install libxmu-dev libxi-dev freeglut3-dev  # Debug/Tests

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

Use [libjpeg-turbo](http://libjpeg-turbo.virtualgl.org/) for `rgb_mjpeg` stream support with decent performance!
```bash
export CMAKE_PREFIX_PATH=<PATH_TO>/libjpeg-turbo-1.3.1:$CMAKE_PREFIX_PATH
export LD_LIBRARY_PATH=<PATH_TO>/libjpeg-turbo-1.3.1/lib:$LD_LIBRARY_PATH
```

## Set Env

```bash
export RIVLIB_ROOT=<INSTALL_PREFIX>
export VISLIB_ROOT=$RIVLIB_ROOT
export THELIB_ROOT=$RIVLIB_ROOT
```
