#! /usr/bin/sh

set -e

cd -- "$( dirname -- "${BASH_SOURCE[0]}" )"

TARGET=tomate
SRC_DIR=./src
BUILD_DIR=./build

LDLIBS="-lraylib -lGL -lm -lpthread -ldl -lrt -lX11"
CFLAGS="-Wall -std=c99"

SRCS="
$SRC_DIR/tomate.c
$SRC_DIR/button.c
$SRC_DIR/timer.c
$SRC_DIR/task.c
"

mkdir -p $BUILD_DIR

gcc $SRCS $LDLIBS -o $BUILD_DIR/$TARGET
