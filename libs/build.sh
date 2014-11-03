#!/bin/bash
#
# Build script
#
# Debian/Ubuntu:
#   apt-get install libxmu-dev libxi-dev zlib1g-dev uuid-dev cmake

prefix=${1:-"/usr/"}

cd vislib
rm -rf release
mkdir release
cd release
cmake .. -DCMAKE_INSTALL_PREFIX=$prefix
if [ $? -ne 0 ] ; then exit 1; fi
make
if [ $? -ne 0 ] ; then exit 1; fi
cd ..
cd ..

cd thelib++
rm -rf release
mkdir release
cd release
cmake .. -DCMAKE_INSTALL_PREFIX=$prefix
if [ $? -ne 0 ] ; then exit 1; fi
make
if [ $? -ne 0 ] ; then exit 1; fi
cd ..
cd ..

cd rivlib
rm -rf release
mkdir release
cd release
cmake .. -DCMAKE_INSTALL_PREFIX=$prefix
if [ $? -ne 0 ] ; then exit 1; fi
make
if [ $? -ne 0 ] ; then exit 1; fi
# sudo make install
cd ..
cd ..

echo
echo "Note: Install rivlib into $prefix:"
echo "    cd rivlib/release"
echo "    sudo make install"
echo
