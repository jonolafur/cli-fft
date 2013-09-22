#!/bin/bash

function build_target() {
  local BUILD_DIR="$1"
  local BUILD_TYPE="$2"

  ################################################################
  ## SVN version:
  ################################################################
  VERSION=`svnversion ${SRC_DIR}`
  echo "SVN Version to be built: ${VERSION}"

  if [ ! -d ${BUILD_DIR} ];
  then
    echo "The directory: ${BUILD_DIR} does not exist. Creating it:"
    mkdir -p ${BUILD_DIR}
  fi

  cd ${BUILD_DIR}
  rm -rf *
  cmake ${SRC_DIR} -DCMAKE_BUILD_TYPE=${BUILD_TYPE}
  make -j 4

}

SRC_DIR=`readlink -e $1`
ROOT_BUILD_DIR="$2"

if [ ! -d ${SRC_DIR} ];
then
  echo "The directory: ${SRC_DIR} does not exist."
  echo "Please select a valid directory containing the sources as a first argument."
  exit 1
fi

if [ ! -d ${ROOT_BUILD_DIR} ];
then
  echo "Creating root build directory: ${ROOT_BUILD_DIR}"
  mkdir -p ${ROOT_BUILD_DIR}
fi

ROOT_BUILD_DIR=`readlink -e ${ROOT_BUILD_DIR}`

echo "Build root dir: ${ROOT_BUILD_DIR}"

## Checking machine architecture and setting directories according to architecture:
ARCH=`uname -i`
BUILD_DIR_RELEASE="${ROOT_BUILD_DIR}/release32"
BUILD_DIR_DEBUG="${ROOT_BUILD_DIR}/debug32"

echo "Building on a $ARCH machine"
if [ "$ARCH" == "x86_64" ];
then
  BUILD_DIR_RELEASE="${ROOT_BUILD_DIR}/release64"
  BUILD_DIR_DEBUG="${ROOT_BUILD_DIR}/debug64"
fi

echo "Build dirs: ${BUILD_DIR_RELEASE}"
echo "Build dirs: ${BUILD_DIR_DEBUG}"


build_target ${BUILD_DIR_RELEASE} "release"
build_target ${BUILD_DIR_DEBUG} "debug"

