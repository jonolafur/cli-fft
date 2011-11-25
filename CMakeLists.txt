cmake_minimum_required (VERSION 2.6)

project(fft)

enable_testing()

set(CMAKE_VERBOSE_MAKEFILE OFF)
set(BUILD_UTILS "build_utils")

if(CMAKE_COMPILER_IS_GNUCXX)
  set( GCC_WARN_OPTIONS "-Wall -Wextra -pedantic -Weffc++ -Wold-style-cast -Woverloaded-virtual -Wno-pmf-conversions -Wsign-promo -fopenmp")
  set( CMAKE_CXX_FLAGS_DEBUG "-O0 -ggdb3 ${GCC_WARN_OPTIONS}" )
  # The option -O3 will turn optimization.
  # The -static will cause the executable to be statically linked.
  # Now it should no longer be required to specify this when calling cmake.
  # The option -s will remove all symbol table and relocation information from the executable.
  set( CMAKE_CXX_FLAGS_RELEASE "-O3 -static -s ${GCC_WARN_OPTIONS}" )
endif(CMAKE_COMPILER_IS_GNUCXX)

if(MSVC)
    foreach(flag_var CMAKE_C_FLAGS_DEBUG CMAKE_CXX_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE CMAKE_CXX_FLAGS_RELEASE CMAKE_C_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_C_FLAGS_RELWITHDEBINFO CMAKE_CXX_FLAGS_RELWITHDEBINFO) 
        string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
        string(REGEX REPLACE "/MDd" "/MTd" ${flag_var} "${${flag_var}}")
    endforeach(flag_var)
    SET (CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}" CACHE STRING "MSVC C Debug MT flags " FORCE)    
    SET (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}" CACHE STRING "MSVC CXX Debug MT flags " FORCE)
    SET (CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}" CACHE STRING "MSVC C Release MT flags " FORCE)
    SET (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}" CACHE STRING "MSVC CXX Release MT flags " FORCE)
    SET (CMAKE_C_FLAGS_MINSIZEREL "${CMAKE_C_FLAGS_MINSIZEREL}" CACHE STRING "MSVC C Debug MT flags " FORCE)    
    SET (CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL}" CACHE STRING "MSVC C Release MT flags " FORCE)
    SET (CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO}" CACHE STRING "MSVC CXX Debug MT flags " FORCE)    
    SET (CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}" CACHE STRING "MSVC CXX Release MT flags " FORCE)
	
	ADD_DEFINITIONS (/D _CRT_SECURE_NO_WARNINGS /D _SCL_SECURE_NO_WARNINGS)
endif()

####################################################################################
### Definition of a macros #########################################################
## This macro executes cmake (referred to by ${CMAKE_COMMAND}). The option
## -E copy_directory is portable way of copying dirs. I use it for example in the ctest
## directory to place my reference data into the build tree.
macro(COPY_DIR src_dir dest_dir)
  exec_program("${CMAKE_COMMAND}" ARGS
               "-E copy_directory \"${src_dir}\" \"${dest_dir}\"")
endmacro(COPY_DIR)

####################################################################################
### BOOST ##########################################################################
# The project uses the program_options component from boost
# Set some variables to make cmake find and prefer local installations
# Note that this didn't seem to quite work on frigg as during make the
# gcc tried to include the boost headers from /usr/include.

set( BOOST_ROOT "~/bin/boost/" )
set( BOOST_INCLUDEDIR "~/bin/boost/" )

find_package( Boost 1.43 COMPONENTS program_options REQUIRED)
message(STATUS "Include boost:  ${BOOST_INCLUDEDIR}" )
message(STATUS "Will link to Boost: ${Boost_LIBRARIES}" )
message(STATUS "Will include boost libs from here: ${Boost_LIBRARY_DIRS}" )
message(STATUS "Will include boost headers from here: ${Boost_INCLUDE_DIR}" )

#####################################################################################
### Support version generation in source code ####################################### 
find_path( VER_TMPL version.h.tmpl
            PATHS
            ${BUILD_UTILS}/Templates )

find_path( VER_SCRT version.sh
            PATHS
            ${BUILD_UTILS}/Scripts )

message(STATUS "Version script located at: ${VER_SCRT}")
message(STATUS "Copying version file template from: ${VER_TMPL}")

####################################################################################
### FFTW ###########################################################################
# Find the include path to FFTW3 (I think cmake searches in the standard path
# by default, but anyway...)
find_path( INCLUDE_FFTW fftw3.h
           PATHS /usr/include c:/fftw3 c:/fftw )

message(STATUS "Using FFTW3 include directory: ${INCLUDE_FFTW} ")

# Find the FFTW3 library:
# On red hat you need to specify lib64 explicitly. On Ubuntu there is a
# sym-link to lib64 from lib on 64 bit platforms.
find_library( LIB_FFTW
              NAMES fftw3 libfftw3-3
              PATHS /usr/lib
                    /usr/lib64
                    /lib
                    /usr/local/lib 
                    c:/fftw3
                    c:/fftw )

message(STATUS "Will link against FFTW3 library found in: ${LIB_FFTW}" )

####################################################################################
### Adding sources #################################################################
set( VER_DES ${CMAKE_CURRENT_BINARY_DIR} )
set( BIN_NAME fft )
set( INST_DIR ~/bin )

include_directories ( ${VER_DES} 
                      ${INCLUDE_FFTW} )

message(STATUS "Calling SVN version tool from: ${CMAKE_CURRENT_SOURCE_DIR}")

if(WIN32)
add_custom_command( OUTPUT ${VER_DES}/version.h
                    COMMAND SubWCRev.exe ${CMAKE_CURRENT_SOURCE_DIR} ${VER_TMPL}/version.h.tmpl 
                            ${VER_DES}/version.h )
else(WIN32)
add_custom_command( OUTPUT ${VER_DES}/version.h
                    COMMAND ${VER_SCRT}/version.sh ${CMAKE_CURRENT_SOURCE_DIR} ${VER_TMPL} ${VER_DES}  )
endif(WIN32)


add_executable (${BIN_NAME}
   src/main.h
   src/main.cpp
   src/fileIO.h
   src/fileIO.cpp
   src/programOptions.h
   src/programOptions.cpp
   src/fftOptions.h
   src/fftOptions.cpp
   src/tty_cols.h
   src/tty_cols.cpp
   src/fftw_utils.h
   src/fftw_vector.h
   src/fftw_vector.cpp
   ${VER_DES}/version.h)

# For the details on the names of the variables involving the
# finding and using of boost, refer to the FindBoost.cmake 
# module. The following tells the linker to link the
# program_options library file from boost (the program_options
# component from the boost package in cmake speak)

if(WIN32)
  target_link_libraries( ${BIN_NAME} ${LIB_FFTW} )
else(WIN32)
  target_link_libraries( ${BIN_NAME} ${LIB_FFTW} ${Boost_LIBRARIES} )
endif(WIN32)

install( TARGETS ${BIN_NAME} DESTINATION ${INST_DIR} )
message( STATUS "${BIN_NAME} will install to ${INST_DIR}" )


####################################################################################
# The following directories are also included.
# They also contain their own CMakeList.txt files
add_subdirectory(${BUILD_UTILS}/Test-utils/ascii-compare)
add_subdirectory(tests)
add_subdirectory(utils)


