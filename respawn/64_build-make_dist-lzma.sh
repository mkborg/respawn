#!/bin/sh

echo "-------------------------------------------------------------------------------"
X_PWD=$(pwd)

X_BUILD_DIR="${X_PWD}-build"

echo "cd ${X_BUILD_DIR} && make dist-lzma $*"
cd ${X_BUILD_DIR} && make dist-lzma $*
