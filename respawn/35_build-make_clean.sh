#!/bin/bash -eu

echo "-------------------------------------------------------------------------------"
X_PWD=$(pwd)

X_BUILD_DIR="${X_PWD}-build"
X_INSTALL_DIR="${X_PWD}-install"

echo "cd ${X_BUILD_DIR} && make clean $*"
cd ${X_BUILD_DIR} && make clean $*
