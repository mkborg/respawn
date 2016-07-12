#!/bin/bash -eu

echo "-------------------------------------------------------------------------------"
X_PWD=$(pwd)

X_PREFIX="--prefix=/"

X_CONFIGURE_OPTIONS+=" ${X_PREFIX}"
echo "X_CONFIGURE_OPTIONS='${X_CONFIGURE_OPTIONS}'"

X_SRC_DIR_BASENAME=$(basename ${X_PWD})
echo "X_SRC_DIR_BASENAME='${X_SRC_DIR_BASENAME}'"

X_BUILD_DIR="../${X_SRC_DIR_BASENAME}-build"
echo "X_BUILD_DIR='${X_BUILD_DIR}'"

if [ ! -d ${X_BUILD_DIR} ] ; then
    echo mkdir ${X_BUILD_DIR}
    mkdir ${X_BUILD_DIR}
fi

echo cd ${X_BUILD_DIR}
cd ${X_BUILD_DIR}

echo ../${X_SRC_DIR_BASENAME}/configure ${X_CONFIGURE_OPTIONS} $*
../${X_SRC_DIR_BASENAME}/configure ${X_CONFIGURE_OPTIONS} $*
