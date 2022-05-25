#!/usr/bin/env bash

set -e

mkdir -p bin
cd bin
cmake -DCMAKE_BUILD_TYPE=Debug .. && make
cp test ..
cd ..
./test
