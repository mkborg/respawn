#!/bin/bash -eu

echo "-------------------------------------------------------------------------------"
X_PWD=$(pwd)

X_BUILD_DIR="${X_PWD}-build"
X_INSTALL_DIR="${X_PWD}-install"

set -x
DESTDIR=${X_INSTALL_DIR} make -C ${X_BUILD_DIR} install $*
