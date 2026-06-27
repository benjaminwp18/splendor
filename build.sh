#!/bin/bash
set -e

if [ ! -d "build" ]; then
    cmake -B build
fi

cd build
make
cd ..
