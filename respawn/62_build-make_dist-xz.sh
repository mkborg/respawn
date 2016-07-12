#!/bin/bash -eu

echo "-------------------------------------------------------------------------------"
X_PWD=$(pwd)

X_BUILD_DIR="${X_PWD}-build"

(
    cd ${X_BUILD_DIR} && make dist-xz $*
)
