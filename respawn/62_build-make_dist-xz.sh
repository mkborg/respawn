#!/bin/bash -eu

echo "-------------------------------------------------------------------------------"
X_PWD=$(pwd)

X_BUILD_DIR="${X_PWD}-build"

make -C ${X_BUILD_DIR} dist-xz $*
