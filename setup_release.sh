#!/bin/sh
BUILD_DIR='build'

rm -rf "$BUILD_DIR" \
    && cmake -DCMAKE_BUILD_TYPE=Release -B "$BUILD_DIR" 1>&2 \
    && echo "$BUILD_DIR"
