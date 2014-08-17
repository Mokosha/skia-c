#!/bin/sh

if ! [ "$1" ]; then
	echo "Usage:" >&2
	echo "  $0 SKIA_SRC_DIR" >&2
	exit 1
fi

# e.g.:
#   GYP_DEFINES="skia_arch_width=32" tests/compile_and_run_test.sh ~/src/skia

SKIA_SRC_DIR="$1"

SKIA_C_SRC_DIR="$(cd "$(dirname "$0")/.."; pwd)"

if ! [ -d "$SKIA_SRC_DIR" ]; then
	echo "'$1' not found" >&2
	exit 1
fi

MYTMP="${TMPDIR:-/tmp}/$(id -un)"

set -e -x

mkdir -p "$MYTMP"

COREGYP="${SKIA_SRC_DIR}/gyp/core.gypi"

SKIAC="'${SKIA_C_SRC_DIR}/src/skia.cpp',"

if ! grep -q "$SKIAC" "$COREGYP"; then
	sed "s|\('sources': \[\)|\\1\n  ${SKIAC}|" "$COREGYP" > "$MYTMP/core.gypi"
	mv "$MYTMP/core.gypi" "$COREGYP"
fi

export SKIA_OUT="${MYTMP}/skia_out"

export GYP_DEFINES="skia_shared_lib=1 ${GYP_DEFINES}"

"${SKIA_SRC_DIR}/gyp_skia"

ninja -C "${SKIA_OUT}/Debug" skia_lib

CC=${CC:-cc}
CFLAGS=${CFLAGS:--Wall -Wextra -pedantic}
LFLAGS="-L ${SKIA_OUT}/Debug/lib -lskia $LFLAGS"

"$CC" $CFLAGS $CPPFLAGS -c "${SKIA_C_SRC_DIR}/tests/test_libskia.c" -o "${MYTMP}/test_libskia.o"

"$CC" "${MYTMP}/test_libskia.o" $LFLAGS -o "${MYTMP}/test_libskia"

LD_LIBRARY_PATH="${SKIA_OUT}/Debug/lib" "${MYTMP}/test_libskia"
