skia-c
======

C-level bindings to the Skia graphics library

-   See [https://github.com/google/skia](https://github.com/google/skia)
    for more information.

### Compilation ###

skia.cpp must be compiled into the Skia shared library.  Skia must be
built with 'skia_shared_lib=1' to create the shared library.

Once this modified version of libskia is produced, you can dynamically
link to it with the skia.h header file.

### Warning ###

This is in the proof-of-concept stage now; large portions of the Skia
API are not yet exposed.

### Tests ###

For examples of how to use this code, look in the tests/ directory.
This example assumes that you have the Skia source and all necessary
dependencies.
