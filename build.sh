#!/bin/bash

BUILD_DIR="build"

if [ ! -d "$BUILD_DIR" ]; then
    mkdir "$BUILD_DIR"
    echo "Created build directory"
fi

# If verbose info
#gcc -O0 -g src/*.c -I./headers/ -o build/simple_shell -Wall -Wextra -Werror -v

# Regular build
gcc -O0 -g src/*.c -I./headers/ -o build/simple_shell -Wall

# Show all header files 
#gcc -H -O0 -g src/*.c -I./headers/ -o build/simple_shell -Wall