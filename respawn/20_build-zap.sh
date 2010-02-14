#!/bin/sh

X_PWD=$(pwd)

X_BUILD_DIR="${X_PWD}-build"

echo "chmod -R +w ${X_BUILD_DIR} $*"
chmod -R +w ${X_BUILD_DIR} $*

echo "rm -rf ${X_BUILD_DIR} $*"
rm -rf ${X_BUILD_DIR} $*
