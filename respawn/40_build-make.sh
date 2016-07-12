#!/bin/bash -eu

echo "-------------------------------------------------------------------------------"
X_PWD=$(pwd)

X_BUILD_DIR="${X_PWD}-build"

set -x
make -C ${X_BUILD_DIR} "CPPFLAGS=${CPPFLAGS:-}" "CFLAGS=${CFLAGS:-}" "CXXFLAGS=${CXXFLAGS:-}" $*
