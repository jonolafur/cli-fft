#!/bin/bash

BUILD_ROOT="$1"
BOOST_VER="$2"

## BOOST script variables ##################
## These are derived from the basic configuration
LIB_ROOT="${BUILD_ROOT}/libraries"
DOWNLOAD_DIR="${BUILD_ROOT}/downloads"
BOOST_ROOT_DIR="${LIB_ROOT}/boost_1_${BOOST_VER}_0"
BOOST_URL="http://sourceforge.net/projects/boost/files/boost/1.${BOOST_VER}.0/boost_1_${BOOST_VER}_0.tar.bz2"


###############################################################
## Get Boost:
echo "####################################################################"
echo "## Retrieveing the Boost library from the Internet using wget..."
echo "####################################################################"
wget --continue --directory-prefix=${DOWNLOAD_DIR} ${BOOST_URL}

echo "####################################################################"
echo "## Expanding the tar ball..."
echo "####################################################################"
tar --directory=${LIB_ROOT} -xf ${DOWNLOAD_DIR}/boost_1_${BOOST_VER}_0.tar.bz2


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
