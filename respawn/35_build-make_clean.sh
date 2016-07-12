#!/bin/bash -eu

echo "-------------------------------------------------------------------------------"
X_PWD=$(pwd)

X_BUILD_DIR="${X_PWD}-build"
X_INSTALL_DIR="${X_PWD}-install"

set -x
make -C ${X_BUILD_DIR} clean $*
