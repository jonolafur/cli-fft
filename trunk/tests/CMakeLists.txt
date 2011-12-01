cmake_minimum_required(VERSION 2.6)

# Name of the test driver
set( TEST_DRIVER_NAME driver )

# include directories
include_directories (${CMAKE_SOURCE_DIR}/src
                     ${INCLUDE_FFTW} )

## This macro is defined in the top-level CMakeLists file. It places a copy of the
## reference data into the build tree.
copy_dir("${CMAKE_CURRENT_SOURCE_DIR}/ref-data" "${CMAKE_CURRENT_BINARY_DIR}/ref-data" )

############################################################
## First we have the tests contained in the auto generated
## test driver
# Create the testing file. The first two arguments are special:
# The name of the list is Test,
# the test driver is test_driver. The files implementing the actual
# tests follow. Each test file must contain one function named
# after the file name. E.g.: Test_spline.cpp contains a function
# like: int Test_spline(int argc, char* argv[]).
create_test_sourcelist( Tests
	${TEST_DRIVER_NAME}.cpp
	Test_get_field.cpp
	Test_read_file.cpp
)

# Create the test driver target. All test source files are
# linked to the test driver along with the objects from the
# sources under test
add_executable( ${TEST_DRIVER_NAME}
                ${CMAKE_SOURCE_DIR}/src/fileIO.cpp
                ${Tests} )

# Link with external libs
target_link_libraries( ${TEST_DRIVER_NAME} ${LIB_FFTW} )

# Generate a list (TestToRun) with the names of all tests. Exclude the test driver!
set (TestsToRun ${Tests})
list(REMOVE_ITEM TestsToRun ${TEST_DRIVER_NAME}.cpp)
#remove (TestsToRun ${TEST_DRIVER_NAME}.cpp)

# This loop adds all the individual tests as tests. It is implemented by calling
# the test driver executable with the file name of the individual test files
foreach (test ${TestsToRun})
	get_filename_component( TName ${test} NAME_WE )
	add_test (${TName} ${TEST_DRIVER_NAME} ${TName})
endforeach (test)

####################################################################################
## Adding tests that call the project executable (i.e. Rx and Tx)
# Generating a CASM L1 signal:
add_test(Test_fft_help ${CMAKE_BINARY_DIR}/fft -? )

## The test above shall return a message including the relevant peak detection:
set_tests_properties(Test_fft_help
                     PROPERTIES PASS_REGULAR_EXPRESSION
                     "Produce help message")

add_test(Test_fft_version ${CMAKE_BINARY_DIR}/fft -v )

## The test above shall return a message including the relevant peak detection:
set_tests_properties(Test_fft_version
                     PROPERTIES PASS_REGULAR_EXPRESSION
                     "This is fft, version:")

add_test( Test_fft_std_in_out ${CMAKE_SOURCE_DIR}/tests/Test_std_in_out.bash )

add_test( Test_fft_inverse ${CMAKE_BINARY_DIR}/fft -I -x 1 -y 2 -c -N
                         -i ${CMAKE_BINARY_DIR}/tests/Test_fft_std_in_out.txt 
                         -o ${CMAKE_BINARY_DIR}/tests/Test_fft_inverse.txt )

add_test(Test_fft_inverse_Compare ${CMAKE_BINARY_DIR}/ascii_compare
               -t ${CMAKE_BINARY_DIR}/tests/Test_read_file.txt
               -r ${CMAKE_BINARY_DIR}/tests/Test_fft_inverse.txt
               -C 0 -d 1e-6 -D 1e-6
                -C 1 -d 1e-6 -D 1e-6
                -C 2 -d 1e-6 -D 1e-6 )




