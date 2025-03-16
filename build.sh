#!/bin/bash

BUILD_DIR="build"

if [ ! -d "$BUILD_DIR" ]; then
    mkdir "$BUILD_DIR"
    echo "Created build directory"
fi

gcc -O0 -g src/main.c -o build/simple_shell -Wall