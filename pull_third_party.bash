#!/bin/bash
## Build date of MinGW-64 tool chain
MINGW_BUILD_DATE="20130622"
BOOST_MIN_VER="54" #Currently using boost 1.54.0
HOST_ARCH="x86_64" #i686
TARGET_ARCH="i686"

## Retrieveing the local directory:
CURR_DIR=`pwd`
BUILD_ROOT="${CURR_DIR}/build"

if [ -d ${BUILD_ROOT} ]
then
  echo "Placing all tool-chain related artefacts in ${BUILD_ROOT}"
else
  echo "Creating build root directory."
  mkdir -p ${BUILD_ROOT}
fi

TC_ROOT="${BUILD_ROOT}/tool-chains"
LIB_ROOT="${BUILD_ROOT}/libraries"
DOWNLOADS="${BUILD_ROOT}/downloads"

mkdir ${BUILD_ROOT}
mkdir -p ${TC_ROOT}
mkdir -p ${LIB_ROOT}
mkdir -p ${DOWNLOADS}

echo "Tool chains will be located here: ${TC_ROOT}"
echo "Third party libs will be located here: ${LIB_ROOT}"
echo "Downloaded files will be located here: ${DOWNLOADS}"

###############################################################
## 32 bit target mingw-w64 tool chain.
TC_MINGW_URL="http://sourceforge.net/projects/mingw-w64/files/Toolchains%20targetting%20Win32/Automated%20Builds"
TC_ID="mingw-w32-bin_${HOST_ARCH}-linux"
TC_MINGW_PREFIX="${TARGET_ARCH}-w64-mingw32"
TC_MINGW_TAR_BALL="${TC_ID}_${MINGW_BUILD_DATE}.tar.bz2"
TC_MINGW_ROOT="${TC_ROOT}/${TC_ID}"

echo "Retrieving the ${TC_MINGW_PREFIX} tool chain using wget..."

wget --continue --directory-prefix=${DOWNLOADS} ${TC_MINGW_URL}/${TC_MINGW_TAR_BALL}

mkdir -p ${TC_MINGW_ROOT}

echo "Extracting the ${TC_ID} tool chain..."

tar --directory ${TC_MINGW_ROOT} -xf ${DOWNLOADS}/${TC_MINGW_TAR_BALL}

###############################################################
## Get Boost:
BOOST_ID="1_${BOOST_MIN_VER}_0"
BOOST_VER="1.${BOOST_MIN_VER}.0"
BOOST_URL="http://sourceforge.net/projects/boost/files/boost/${BOOST_VER}"
BOOST_TAR_BALL="boost_${BOOST_ID}.tar.bz2"
BOOST_ROOT_DIR="${LIB_ROOT}/boost_${BOOST_ID}"

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
                       using gcc : mingw : ${TC_MINGW_ROOT}/bin/${TC_MINGW_PREFIX}-g++ ;" > project-config.jam
   rm tmp.jam
   ## Building boost libraries with bjam:
   ./b2 -a toolset=gcc target-os=windows --layout=tagged --with-program_options\
        --prefix=${TC_MINGW_ROOT}/${TC_MINGW_PREFIX}/ install
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
   ./configure CC=${TC_MINGW_ROOT}/bin/${TC_MINGW_PREFIX}-gcc \
        --prefix=${TC_MINGW_ROOT}/${TC_MINGW_PREFIX}\
        --host=${TC_MINGW_PREFIX}\
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
echo "Generating a cmake tool chain file for ${TC_MINGW_PREFIX}:"

cat mingw.cmake.template | sed -e "s:set(TC_PREFIX:& ${TC_MINGW_PREFIX}:" \
                               -e "s:set(CMAKE_FIND_ROOT_PATH:& ${TC_MINGW_ROOT} ${TC_MINGW_ROOT}/${TC_MINGW_PREFIX}:"\
                               > mingw32.cmake
mkdir -p build/mingw32
cd build/mingw32

echo "Starting cmake with the mingw32.cmake tool chain file to build for ${TC_MINGW_PREFIX}:"

rm -rf * && cmake -DCMAKE_TOOLCHAIN_FILE=../../mingw32.cmake -DCMAKE_BUILD_TYPE=release ../../



