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

If you use older `gcc` versions such as `4.6.2` on Hypnos, modify the file `/libs/vislib/CMakeLists.txt:22` to use `-std=c++0x` instead of `-std=c++11`.

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
