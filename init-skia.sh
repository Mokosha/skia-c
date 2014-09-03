#!/bin/bash

# The revision of Skia that we work against
SKIA_REV=e5ea500d4714

# Make sure that depot_tools directory is initialized
git submodule update

if [ ! -d skia ]; then
    mkdir skia
    cd skia

    echo "Checking out revision ${SKIA_REV}..."
    PATH=../depot_tools:$PATH gclient config --name . --unmanaged https://skia.googlesource.com/skia.git
    PATH=../depot_tools:$PATH gclient sync
    git checkout ${SKIA_REV}

    echo "Adding our skia.cpp glue into core.gypi..."
    TMPFILE=`mktemp`
    if ! grep -q "skia.cpp" "gyp/core.gypi"; then
        sed -e "s|\('sources': \[\)|\\1\n        '<(skia_src_path)/../../src/skia.cpp',|" gyp/core.gypi > $TMPFILE
        mv $TMPFILE gyp/core.gypi
    else
        rm -f $TMPFILE
    fi

    echo "Building Skia..."
    ./gyp_skia
    BUILDTYPE=Release GYP_DEFINES="skia_shared_lib=1" make

    echo "Complete!"
else
    echo "Top-level skia directory already exists!"
fi
