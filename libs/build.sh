#!/bin/bash
#
# Build script
#

cd vislib
rm -rf release
mkdir release
cd release
cmake ..
if [ $? -ne 0 ] ; then exit 1; fi
make
if [ $? -ne 0 ] ; then exit 1; fi
cd ..
cd ..

cd thelib++
rm -rf release
mkdir release
cd release
cmake ..
if [ $? -ne 0 ] ; then exit 1; fi
make
if [ $? -ne 0 ] ; then exit 1; fi
cd ..
cd ..

cd rivlib
rm -rf release
mkdir release
cd release
cmake ..
if [ $? -ne 0 ] ; then exit 1; fi
make
if [ $? -ne 0 ] ; then exit 1; fi
# sudo make install
cd ..
cd ..

echo
echo "Note: Install rivlib:"
echo "    cd rivlib/release"
echo "    sudo make install"
echo
