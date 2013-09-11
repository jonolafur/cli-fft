#!/bin/bash

function build_target() {
  BUILD_DIR="$1"
  BUILD_TYPE="$2"

  ################################################################
  ## SVN version:
  ################################################################
  VERSION=`svnversion .`
  echo "SVN Version to be built: ${VERSION}"

  if [ ! -d ${BUILD_DIR} ];
  then
    echo "The directory: ${BUILD_DIR} does not exist. Creating it:"
    mkdir -p ${BUILD_DIR}
  fi

  cd ${BUILD_DIR}
  rm -rf *
  cmake ../../ -DCMAKE_BUILD_TYPE=${BUILD_TYPE}
  make

}

ARCH=`uname -i`
ROOT_DIR=`pwd`
BUILD_DIR_RELEASE="${ROOT_DIR}/build/release32"
BUILD_DIR_DEBUG="${ROOT_DIR}/build/debug32"

echo "Building on a $ARCH machine"
if [ "$ARCH" == "x86_64" ];
then
  BUILD_DIR_RELEASE="${ROOT_DIR}/build/release64"
  BUILD_DIR_DEBUG="${ROOT_DIR}/build/debug64"
fi


build_target ${BUILD_DIR_RELEASE} "release"
build_target ${BUILD_DIR_DEBUG} "debug"

