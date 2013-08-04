#!/bin/bash

## Basic configuration #####################
## Thes variables con be changed to acount for later versions of libs and tool chains
## Build date of MinGW-64 tool chain
HOST_ARCH="x86_64"             # Set to "i686" if building on a 32 bit Linux
NUM_BIT="32"                   # Default to 32 bit windows
TARGET_ARCH="x86_64"           # Set to "i686" if building for a Win32 target
MINGW_BUILD_DATE="20130622"    # You need to check on sourceforge for the availability of a later toolchain if you want an earlier one.
BOOST_MIN_VER="54"             # Currently using boost 1.54.0

################################################################
## Checking which arhitecture (32 or 64 bits) the user wants:
################################################################
if [ "$1" == "32" ];
then
  NUM_BIT="32"
  TARGET_ARCH="i686"           # Set to "i686" if building for a Win32 target
fi

if [ "$1" == "64" ];
then
  NUM_BIT="64"
  TARGET_ARCH="x86_64"           # Set to "i686" if building for a Win32 target
fi

echo "####################################################################"
echo "## Configuring for ${NUM_BIT}-bit windows target."
echo "####################################################################"

################################################################
## Setting up the build directories:
################################################################
CURR_DIR=`pwd`   ## Retrieveing the local directory:
BUILD_ROOT="${CURR_DIR}/build"

if [ -d ${BUILD_ROOT} ]
then
  echo "####################################################################"
  echo "## Placing all tool-chain related artefacts in ${BUILD_ROOT}"
  echo "####################################################################"
else
  echo "####################################################################"
  echo "## Creating build root directory."
  echo "####################################################################"
  mkdir -p ${BUILD_ROOT}
fi

TC_ROOT="${BUILD_ROOT}/tool-chains"
LIB_ROOT="${BUILD_ROOT}/libraries"
DOWNLOADS="${BUILD_ROOT}/downloads"

mkdir ${BUILD_ROOT}
mkdir -p ${TC_ROOT}
mkdir -p ${LIB_ROOT}
mkdir -p ${DOWNLOADS}

echo "####################################################################"
echo "## Tool chains will be located here: ${TC_ROOT}"
echo "## Third party libs will be located here: ${LIB_ROOT}"
echo "## Downloaded files will be located here: ${DOWNLOADS}"
echo "####################################################################"

################################################################
## Tool chain script variables ##################
## These are derived from the basic configuration
TC_MINGW_URL="http://sourceforge.net/projects/mingw-w64/files/Toolchains%20targetting%20Win${NUM_BIT}/Automated%20Builds"
TC_ID="mingw-w${NUM_BIT}-bin_${HOST_ARCH}-linux"
TC_MINGW_PREFIX="${TARGET_ARCH}-w64-mingw32"
TC_MINGW_TAR_BALL="${TC_ID}_${MINGW_BUILD_DATE}.tar.bz2"
TC_MINGW_ROOT="${TC_ROOT}/${TC_ID}"

## BOOST script variables ##################
## These are derived from the basic configuration
BOOST_ID="1_${BOOST_MIN_VER}_0"
BOOST_VER="1.${BOOST_MIN_VER}.0"
BOOST_URL="http://sourceforge.net/projects/boost/files/boost/${BOOST_VER}"
BOOST_TAR_BALL="boost_${BOOST_ID}.tar.bz2"
BOOST_ROOT_DIR="${LIB_ROOT}/boost_${BOOST_ID}"

## FFTW  script variables ##################
## These are derived from the basic configuration
FFTW_SITE="http://fftw.org"
FFTW_TAR_BALL="fftw-3.3.3.tar.gz"
FFTW_ROOT_DIR="${LIB_ROOT}/fftw-3.3.3"

###############################################################
## Getting the mingw64 tool chain.
echo "####################################################################"
echo "## Retrieving the ${TC_MINGW_PREFIX} tool chain from the Internet using wget..."
echo "####################################################################"

wget --continue --directory-prefix=${DOWNLOADS} ${TC_MINGW_URL}/${TC_MINGW_TAR_BALL}
mkdir -p ${TC_MINGW_ROOT}

echo "####################################################################"
echo "## Extracting the ${TC_ID} tool chain..."
echo "####################################################################"

tar --directory ${TC_MINGW_ROOT} -xf ${DOWNLOADS}/${TC_MINGW_TAR_BALL}

###############################################################
## Get Boost:
echo "####################################################################"
echo "## Retrieveing the Boost library from the Internet using wget..."
echo "####################################################################"
wget --continue --directory-prefix=${DOWNLOADS} ${BOOST_URL}/${BOOST_TAR_BALL}

echo "####################################################################"
echo "## Expanding the tar ball..."
echo "####################################################################"
tar --directory=${LIB_ROOT} -xf ${DOWNLOADS}/${BOOST_TAR_BALL}

if [ -d ${BOOST_ROOT_DIR} ]
then
   echo "####################################################################"
   echo "## Changing into the Boost root directory: ${BOOST_ROOT_DIR}"
   echo "## and performing boost boot-strap process..."
   echo "####################################################################"

   cd ${BOOST_ROOT_DIR}
   ./bootstrap.sh

   ## Manipulating the project-config.jam file. There may be a better way to do this
   ## using user-config.jam, but it didn't work for me. It is important to insert the
   ## using gcc : ... line before the
   ##   if ! gcc in [ feature.values <toolset> ]... clause. I append it immedeately
   ## after the "import feature ;" line: 
   echo "####################################################################"
   echo "## Manipulating the project-config.jam to make sure cross compile works..."
   echo "####################################################################"

   cp project-config.jam tmp.jam
   cat tmp.jam | sed "/import feature ;/a\
                       using gcc : mingw : ${TC_MINGW_ROOT}/bin/${TC_MINGW_PREFIX}-g++ ;" > project-config.jam
   rm tmp.jam
 
   echo "####################################################################"
   echo "## Building boost libraries with bjam..."
   echo "####################################################################"
   ./b2 -a toolset=gcc target-os=windows --layout=tagged --with-program_options\
        --prefix=${TC_MINGW_ROOT}/${TC_MINGW_PREFIX}/ install
   cd  ${CURR_DIR} # Back to the original 

   echo "####################################################################"
   echo "## Finished generating boost library."
   echo "####################################################################"

else
   echo "####################################################################"
   echo "## Failed to locate the boost root directory: ${BOOST_ROOT_DIR}"
   echo "## Something went wrong in getting and extracting the boost files..."
   echo "####################################################################"

   exit 1
fi

###########################################################################
## Get FFTW:
echo "####################################################################"
echo "## Retrieving FFTW 3 from the internet..."
echo "####################################################################"

wget --continue --directory-prefix=${DOWNLOADS} ${FFTW_SITE}/${FFTW_TAR_BALL}

echo "####################################################################"
echo "## Finished Retrieving FFTW 3 from the internet..."
echo "## Extracting FFTW 3 sources to ${LIB_ROOT}..."
echo "####################################################################"
tar --directory=${LIB_ROOT} -xf ${DOWNLOADS}/${FFTW_TAR_BALL}

if [ -d ${FFTW_ROOT_DIR} ]
then
   echo "####################################################################"
   echo "## Changing into the fftw root directory: ${FFTW_ROOT_DIR}"
   echo "## Starting FFTW 3 build for target: ${TARGET_ARCH}"
   echo "####################################################################"

   cd ${FFTW_ROOT_DIR}
   ./configure CC=${TC_MINGW_ROOT}/bin/${TC_MINGW_PREFIX}-gcc \
        --prefix=${TC_MINGW_ROOT}/${TC_MINGW_PREFIX}\
        --host=${TC_MINGW_PREFIX}\
        -with-our-malloc --with-windows-f77-mangling --disable-shared --enable-static --enable-threads\
        --with-combined-threads --with-incoming-stack-boundary=2
   make -j
   make install
   cd  ${CURR_DIR} # Back to the original 
else
   echo "####################################################################"
   echo "## Failed to locate the FFTW root directory: ${FFTW_ROOT_DIR}"
   echo "## Something went wrong in getting and extracting the fftw sources..."
   echo "####################################################################"
   exit 1
fi

###########################################################################
## Prepare tool chain file for cmake:
## Constructing the tool chain file from the template mingw.cmake.template. Here we need to
## insert the corresponding values for the tool chain:
echo "####################################################################"
echo "## Generating a cmake tool chain file for ${TC_MINGW_PREFIX}:"
echo "####################################################################"

cat mingw.cmake.template | sed -e "s:set(TC_PREFIX:& ${TC_MINGW_PREFIX}:" \
                               -e "s:set(CMAKE_FIND_ROOT_PATH:& ${TC_MINGW_ROOT} ${TC_MINGW_ROOT}/${TC_MINGW_PREFIX}:"\
                               > mingw_${TARGET_ARCH}.cmake

mkdir -p build/mingw64_${TARGET_ARCH}
cd build/mingw64_${TARGET_ARCH}

echo "####################################################################"
echo "## Starting cmake with the mingw_${TARGET_ARCH}.cmake tool chain file to build for ${TC_MINGW_PREFIX}:"
echo "####################################################################"

rm -rf * && cmake -DCMAKE_TOOLCHAIN_FILE=../../mingw_${TARGET_ARCH}.cmake -DCMAKE_BUILD_TYPE=release ../../

echo "####################################################################"
echo "## Finshed. If everything went well, you can go to the directory:"
echo "## build/mingw64_${TARGET_ARCH}"
echo "## and issue \"make\"."
echo "####################################################################"


