#!/bin/sh

set -x
sed -ie 's/libs apps/libs/' Makefile

make XCFLAGS="-I$1 -Iinclude/mupdf" libs build=release verbose=yes prefix=/usr $2
