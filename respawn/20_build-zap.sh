#!/bin/bash -eu

X_PWD=$(pwd)

X_BUILD_DIR="${X_PWD}-build"

set -x
chmod -R +w ${X_BUILD_DIR} $*
rm -rf ${X_BUILD_DIR} $*
