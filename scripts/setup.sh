#!/bin/bash

# Define and go to source directory
SOURCE_DIR=$(dirname "$(realpath "$0")")/../
cd "$SOURCE_DIR" || {
    echo "Failed to change directory to $SOURCE_DIR";
    exit 1;
}

# Make build and binary folders
mkdir -p bin
mkdir -p build

# Setup and buildb
cd build
cmake -S "$SOURCE_DIR" -B .
make
