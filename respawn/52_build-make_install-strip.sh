#!/bin/bash -eu

echo "-------------------------------------------------------------------------------"
X_PWD=$(pwd)

X_BUILD_DIR="${X_PWD}-build"
X_INSTALL_DIR="${X_PWD}-install-strip"

echo "cd ${X_BUILD_DIR} && DESTDIR=${X_INSTALL_DIR} make install-strip $*"
cd ${X_BUILD_DIR} && DESTDIR=${X_INSTALL_DIR} make install-strip $*
