#!/bin/bash
#
# Build script
#
# Debian/Ubuntu:
#   apt-get install libxmu-dev libxi-dev freeglut3-dev zlib1g-dev uuid-dev cmake

cd vislib
rm -rf release
mkdir release
cd release
cmake -DBUILD_TESTS=1 ..
if [ $? -ne 0 ] ; then exit 1; fi
make
if [ $? -ne 0 ] ; then exit 1; fi
cd ..
rm -rf debug
mkdir debug
cd debug
cmake -DBUILD_TESTS=1 -DCMAKE_BUILD_TYPE=Debug ..
if [ $? -ne 0 ] ; then exit 1; fi
make
if [ $? -ne 0 ] ; then exit 1; fi
cd ..
cd ..

cd thelib++
rm -rf release
mkdir release
cd release
cmake -DBUILD_TESTS=1 ..
if [ $? -ne 0 ] ; then exit 1; fi
make
if [ $? -ne 0 ] ; then exit 1; fi
cd ..
rm -rf debug
mkdir debug
cd debug
cmake -DBUILD_TESTS=1 -DCMAKE_BUILD_TYPE=Debug ..
if [ $? -ne 0 ] ; then exit 1; fi
make
if [ $? -ne 0 ] ; then exit 1; fi
cd ..
cd ..

cd rivlib
rm -rf release
mkdir release
cd release
cmake -DBUILD_TESTS=1 ..
if [ $? -ne 0 ] ; then exit 1; fi
make
if [ $? -ne 0 ] ; then exit 1; fi
cd ..
rm -rf debug
mkdir debug
cd debug
cmake -DBUILD_TESTS=1 -DCMAKE_BUILD_TYPE=Debug ..
if [ $? -ne 0 ] ; then exit 1; fi
make
if [ $? -ne 0 ] ; then exit 1; fi
cd ..
cd ..
