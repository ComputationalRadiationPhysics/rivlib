#!/bin/bash
#
# Build script
#

cd vislib
mkdir release
cd release
cmake ..
make
cd ..
cd ..

cd thelib++
mkdir release
cd release
cmake ..
make
cd ..
cd ..

cd rivlib
mkdir release
cd release
cmake ..
make
cd ..
cd ..

