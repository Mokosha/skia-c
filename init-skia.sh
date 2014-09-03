#!/bin/bash

# Make sure that depot_tools directory is initialized
git submodule update

if [ ! -d skia ]; then
    mkdir skia
    cd skia
    PATH=../depot_tools:$PATH gclient config --name . --unmanaged https://skia.googlesource.com/skia.git
    PATH=../depot_tools:$PATH gclient sync
    git checkout e5ea500d4714
fi
