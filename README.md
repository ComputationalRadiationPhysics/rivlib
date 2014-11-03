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

## Set Env

```bash
export RIVLIB_ROOT=<INSTALL_PREFIX>
export VISLIB_ROOT=$RIVLIB_ROOT
export THELIB_ROOT=$RIVLIB_ROOT
```
