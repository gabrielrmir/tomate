#! /usr/bin/sh

set -e

ROOT=$( dirname -- "${BASH_SOURCE[0]:-$0}" )
SRC=$ROOT/src
BUILD=$ROOT/build

mkdir -p $BUILD

INCLUDE=-I$HOME/deps/raylib/src/
LIBDIR=-L$HOME/deps/raylib/src/

SRCFILES="$SRC/tomate.c $SRC/button.c $SRC/timer.c"

gcc \
  $SRCFILES \
  -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 \
  -o $BUILD/tomate
