#!/bin/bash

# Create build directory if it doesn't exist
mkdir -p build

# Optimization: 
# -g: include debug information
# -Wall: enable all common warnings
# -Werror: treat warnings as errors (optional, but good for Handmade style)

echo "Building Handmade Hero..."

gcc -g code/linux_handmade.c -o build/handmade_hero -lX11

echo "Build complete."
