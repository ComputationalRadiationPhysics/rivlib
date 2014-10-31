#!/bin/bash
#
# Build script
#

cd vislib
mkdir release
cd release
cmake -DBUILD_TESTS=1 ..
make
cd ..
mkdir debug
cd debug
cmake -DBUILD_TESTS=1 -DCMAKE_BUILD_TYPE=Debug ..
make
cd ..
cd ..

cd thelib++
mkdir release
cd release
cmake -DBUILD_TESTS=1 ..
make
cd ..
mkdir debug
cd debug
cmake -DBUILD_TESTS=1 -DCMAKE_BUILD_TYPE=Debug ..
make
cd ..
cd ..

cd rivlib
mkdir release
cd release
cmake -DBUILD_TESTS=1 ..
make
cd ..
mkdir debug
cd debug
cmake -DBUILD_TESTS=1 -DCMAKE_BUILD_TYPE=Debug ..
make
cd ..
cd ..

