#!/bin/bash

## Retrieveing the local directory:
CURR_DIR=`pwd`
BUILD_DIR="${CURR_DIR}/build"
TC_ROOT="${BUILD_DIR}/tool-chains"
LIB_ROOT="${BUILD_DIR}/libraries"
DOWNLOADS="${BUILD_DIR}/downloads"

mkdir ${BUILD_DIR}
mkdir -p ${TC_ROOT}
mkdir -p ${LIB_ROOT}
mkdir -p ${DOWNLOADS}

echo "Tool chains will be located here: ${TC_ROOT}"
echo "Third party libs will be located here: ${LIB_ROOT}"
echo "Downloaded files will be located here: ${DOWNLOADS}"

###############################################################
## mingw32-w64 tool chain.
TC_URL="http://sourceforge.net/projects/mingw-w64/files/Toolchains%20targetting%20Win32/Automated%20Builds"
TC_MINGW32_PREFIX="i686-w64-mingw32"
TC_MINGW32_TAR_BALL="mingw-w32-bin_i686-linux_20121015.tar.bz2"
TC_MINGW32_ROOT="${TC_ROOT}/mingw-w32-bin_i686-linux"

echo "Retrieving the ${TC_MINGW32_PREFIX} tool chain using wget..."

wget --continue --directory-prefix=${DOWNLOADS} ${TC_URL}/${TC_MINGW32_TAR_BALL}

mkdir -p ${TC_MINGW32_ROOT}

echo "Extracting the mingw-w32-bin_i686-linux tool chain..."

tar --directory ${TC_MINGW32_ROOT} -xf ${DOWNLOADS}/${TC_MINGW32_TAR_BALL}

###############################################################
## Get Boost:
BOOST_URL="http://sourceforge.net/projects/boost/files/boost/1.52.0"
BOOST_TAR_BALL="boost_1_52_0.tar.bz2"
BOOST_ROOT_DIR="${LIB_ROOT}/boost_1_52_0"

wget --continue --directory-prefix=${DOWNLOADS} ${BOOST_URL}/${BOOST_TAR_BALL}

tar --directory=${LIB_ROOT} -xf ${DOWNLOADS}/${BOOST_TAR_BALL}

if [ -d ${BOOST_ROOT_DIR} ]
then
   echo "Changing into the Boost root directory: ${BOOST_ROOT_DIR}"
   cd ${BOOST_ROOT_DIR}
   ./bootstrap.sh

   ## Manipulating the project-config.jam file. There may be a better way to do this
   ## using user-config.jam, but it didn't work for me. It is important to insert the
   ## using gcc : ... line before the
   ##   if ! gcc in [ feature.values <toolset> ]... clause. I append it immedeately
   ## after the "import feature ;" line: 
   cp project-config.jam tmp.jam
   cat tmp.jam | sed "/import feature ;/a\
                       using gcc : mingw : ${TC_MINGW32_ROOT}/bin/${TC_MINGW32_PREFIX}-g++ ;" > project-config.jam
   rm tmp.jam
   ## Building boost libraries with bjam:
   ./b2 -a toolset=gcc target-os=windows --layout=tagged --with-program_options\
        --prefix=${TC_MINGW32_ROOT}/${TC_MINGW32_PREFIX}/ install
   cd  ${CURR_DIR} # Back to the original 
else
   echo "Failed to locate the boost root directory: ${BOOST_ROOT_DIR}"
   echo "Something went wrong in getting and extracting the boost files..."
   exit 1
fi

###########################################################################
## Get FFTW:
FFTW_SITE="http://fftw.org"
FFTW_TAR_BALL="fftw-3.3.3.tar.gz"
FFTW_ROOT_DIR="${LIB_ROOT}/fftw-3.3.3"

wget --continue --directory-prefix=${DOWNLOADS} ${FFTW_SITE}/${FFTW_TAR_BALL}

tar --directory=${LIB_ROOT} -xf ${DOWNLOADS}/${FFTW_TAR_BALL}

if [ -d ${FFTW_ROOT_DIR} ]
then
   echo "Changing into the fftw root directory: ${FFTW_ROOT_DIR}"
   cd ${FFTW_ROOT_DIR}
   ./configure CC=${TC_MINGW32_ROOT}/bin/${TC_MINGW32_PREFIX}-gcc \
        --prefix=${TC_MINGW32_ROOT}/${TC_MINGW32_PREFIX}\
        --host=${TC_MINGW32_PREFIX}\
        -with-our-malloc --with-windows-f77-mangling --disable-shared --enable-static --enable-threads\
        --with-combined-threads --with-incoming-stack-boundary=2
   make -j
   make install
   cd  ${CURR_DIR} # Back to the original 
else
   echo "Failed to locate the FFTW root directory: ${FFTW_ROOT_DIR}"
   echo "Something went wrong in getting and extracting the fftw sources..."
   exit 1
fi

###########################################################################
## Prepare tool chain file for cmake:
## Constructing the tool chain file from the template mingw.cmake.template. Here we need to
## insert the corresponding values for the tool chain:
echo "Generating a cmake tool chain file for ${TC_MINGW32_PREFIX}:"

cat mingw.cmake.template | sed -e "s:set(TC_PREFIX:& ${TC_MINGW32_PREFIX}:" \
                               -e "s:set(CMAKE_FIND_ROOT_PATH:& ${TC_MINGW32_ROOT} ${TC_MINGW32_ROOT}/${TC_MINGW32_PREFIX}:"\
                               > mingw32.cmake
mkdir -p build/mingw32
cd build/mingw32

echo "Starting cmake with the mingw32.cmake tool chain file to build for ${TC_MINGW32_PREFIX}:"

rm -rf * && cmake -DCMAKE_TOOLCHAIN_FILE=../../mingw32.cmake -DCMAKE_BUILD_TYPE=release ../../



