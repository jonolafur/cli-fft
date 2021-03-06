cmake_minimum_required (VERSION 2.6)

project(ascii_compare)

set( BIN_NAME ascii_compare )

enable_testing()

set(CMAKE_VERBOSE_MAKEFILE OFF)

if(CMAKE_COMPILER_IS_GNUCXX)
  set( GCC_WARN_OPTIONS "-Wall -Wextra -pedantic -Weffc++ -Wold-style-cast -Woverloaded-virtual -Wno-pmf-conversions -Wsign-promo")
  set( CMAKE_CXX_FLAGS_DEBUG "-O0 -ggdb3 ${GCC_WARN_OPTIONS}" )
  # The option -O3 will turn optimization.
  # The -static will cause the executable to be statically linked.
  # Now it should no longer be required to specify this when calling cmake.
  # The option -s will remove all symbol table and relocation information from the executable.
  set( CMAKE_CXX_FLAGS_RELEASE "-O3 -static -s ${GCC_WARN_OPTIONS}" )
endif(CMAKE_COMPILER_IS_GNUCXX)

####################################################################################
### BOOST ###########################################################################
# The project uses the program_options component from boost
# Set some variables to make cmake find and prefer local installations
# Note that this didn't seem to quite work on frigg as during make the
# gcc tried to include the boost headers from /usr/include.

#set( BOOST_ROOT "~/bin/boost/" )
#set( BOOST_INCLUDEDIR "~/bin/boost/" )

find_package( Boost COMPONENTS program_options REQUIRED)

message(STATUS "Include boost:  ${BOOST_INCLUDEDIR}" )
message(STATUS "Will link to Boost: ${Boost_LIBRARIES}" )
message(STATUS "Will include boost libs from here: ${Boost_LIBRARY_DIRS}" )
message(STATUS "Will include boost headers from here: ${Boost_INCLUDE_DIR}" )

#####################################################################################
## Generating version file:
set( VER_DES ${CMAKE_CURRENT_BINARY_DIR} )
include_directories ( ${VER_DES} )

find_path( VER_TMPL version.h.tmpl
            PATHS
            ../../Templates )

find_path( VER_SCRT version.sh
            PATHS
            ../../Scripts )

message(STATUS "Version script located at: ${VER_SCRT}")
message(STATUS "Copying version file template from: ${VER_TMPL}")
message(STATUS "Calling SVN version tool from: ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "Location of the version.h file: ${VER_DES}" )

if(CMAKE_SYSTEM_NAME EQUAL "Windows")
add_custom_command( OUTPUT ${VER_DES}/version.h
                    COMMAND SubWCRev.exe ${CMAKE_CURRENT_SOURCE_DIR} ${VER_TMPL}/version.h.tmpl
                            ${VER_DES}/version.h )
else(CMAKE_SYSTEM_NAME EQUAL "Windows")
add_custom_command( OUTPUT ${VER_DES}/version.h
                    COMMAND ${VER_SCRT}/version.py ${CMAKE_CURRENT_SOURCE_DIR} ${VER_TMPL} ${VER_DES}  )
endif(CMAKE_SYSTEM_NAME EQUAL "Windows")

###########################################################################
## Add executable:
## To make sure the executable is reachable at a standard location, I set the
## CMAKE_RUNTIME_OUTPUT_DIRECTORY variable to CMAKE_BINARY_DIR. This variable
## (CMAKE_RUNTIME_OUTPUT_DIRECTORY) will then be used to set the corresponding
## target property. Afterwards I unset this variable, otherwise every target
## below this point will also turn up at CMAKE_BINARY_DIR.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_executable (${BIN_NAME}
   ${VER_DES}/version.h
   get_fields.h
   ascii_compare.h
   ascii_compare.cpp)

unset(CMAKE_RUNTIME_OUTPUT_DIRECTORY)

message(STATUS "### !!! Using ascii_compare!!!" )
message(STATUS "### The ascii_compare tool was compiled and is located in the root directory" )
message(STATUS "### of your build tree. This is controlled by the variable CMAKE_BINARY_DIR" )
message(STATUS "### The current location is: ${CMAKE_BINARY_DIR}" )
message(STATUS "### When you write your tests, make sure to point to this directory." )
message(STATUS "### I think ctest will search this location automatically, but I'm not sure." )

# For the details on the names of the variables involving the
# finding and using of boost, refer to the FindBoost.cmake
# module. The following tells the linker to link the
# program_options library file from boost (the program_options
# component from the boost package in cmake speak)

if(MSVC)
  target_link_libraries( ${BIN_NAME} )
else(MSVC)
  target_link_libraries( ${BIN_NAME} ${Boost_LIBRARIES} )
endif(MSVC)

add_subdirectory (test)



