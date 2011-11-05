cmake_minimum_required(VERSION 2.6)

# include directories
include_directories ( ../ )

############################################################
## First we have the tests contained in the auto generated
## test driver
# Create the testing file. The first two arguments are specialk:
# The name of the list is Test,
# the test driver is test_driver. The files implementing the actual
# tests follow. Each test file must contain one function named
# after the file name. E.g.: Test_spline.cpp contains a function
# like: int Test_spline(int argc, char* argv[]).
create_test_sourcelist( Tests
	test_driver_ascii_compare.cpp
	Test_get_fields.cpp
	Test_gen_data.cpp
)

# Create the test driver target. All test source files are
# linked to the test driver along with the objects from the
# sources under test
add_executable( test_driver_ascii_compare
                ${Tests}
                ../get_fields.h
)

# Link with external libs
target_link_libraries( test_driver_ascii_compare )

# Generate a list (TestToRun) with the names of all tests. Exclude the test driver!
set (TestsToRun ${Tests})
remove (TestsToRun test_driver_ascii_compare.cpp)

# This loop adds all the individual tests as tests. It is implemented by calling
# the test driver executable with the file name of the individual test files
foreach (test ${TestsToRun})
	get_filename_component( TName ${test} NAME_WE )
	add_test (${TName} test_driver_ascii_compare ${TName})
endforeach (test)


#########################
## Just make sure the application starts, and stops without a hitch:
add_test(Test_Compare_Files ${CMAKE_BINARY_DIR}/ascii_compare )

#########################
## Just make sure the application starts, and stops without a hitch:
add_test(Test_Compare_Files_Version ${CMAKE_BINARY_DIR}/ascii_compare --version)
## The test above shall return a version message:
set_tests_properties(Test_Compare_Files_Version
                     PROPERTIES PASS_REGULAR_EXPRESSION
                     "This is ascii_compare, version:")

#########################
## Perform the test on an error free text file:
add_test(Test_Compare_Files_No_Error ${CMAKE_BINARY_DIR}/ascii_compare --test-file test_file_no_error.txt
	                                  --reference-file ref_file.txt -C 0 -C 1 -C 2 -C 4
	                                  -d 1.0e-20 -d 1.0e-20 -d 1.0e-20 -d 1.0e-20
	                                  -D 1.0e-20 -D 1.0e-20 -D 1.0e-20 -D 1.0e-20
	                                  --num-expected-col 200 )

#########################
## Perform the test on an error text file:
add_test(Test_Compare_Files_With_Error_1 ${CMAKE_BINARY_DIR}/ascii_compare --test-file test_file_w_error.txt
	                                  --reference-file ref_file.txt -C 0 -C 1 -C 2 -C 4
	                                  -d 1.0e-20 -d 1.0e-20 -d 1.0e-20 -d 1.0e-20
	                                  -D 1.0e-20 -D 1.0e-20 -D 1.0e-20 -D 1.0e-20
	                                  --num-expected-col 200 )
## The test above is supposed to fail. Thus I set the test property WILL_FAIL to true:
set_tests_properties(Test_Compare_Files_With_Error_1 PROPERTIES WILL_FAIL true )

#########################
add_test(Test_Compare_Files_With_Error_2 ${CMAKE_BINARY_DIR}/ascii_compare --test-file test_file_w_error.txt
	                                  --reference-file ref_file.txt -C 0 -C 1 -C 2 -C 4
	                                  -d 1.0e-7 -d 1.0e-20 -d 1.0e-20 -d 1.0e-20
	                                  -D 2.0e-7 -D 1.0e-20 -D 1.0e-20 -D 1.0e-20
	                                  --num-expected-col 200 )
## The test above is supposed to fail. Thus I set the test property WILL_FAIL to true:
set_tests_properties(Test_Compare_Files_With_Error_2 PROPERTIES WILL_FAIL true )

#########################
## This test sets the tolerances such that the deviations between the files are just tolerated:
add_test(Test_Compare_Files_With_Error_3 ${CMAKE_BINARY_DIR}/ascii_compare --test-file test_file_w_error.txt
	                                  --reference-file ref_file.txt -C 0 -C 1 -C 2 -C 4
	                                  -d 1.0e-7 -d 1.0e-7 -d 1.0e-7 -d 1.0e-7
	                                  -D 2.0e-7 -D 2.0e-7 -D 2.0e-7 -D 2.0e-7
	                                  --num-expected-col 200 )
#########################
## This test sets the tolerances such that the deviations between the files are just tolerated:
add_test(Test_Compare_Files_No_Columns ${CMAKE_BINARY_DIR}/ascii_compare --test-file test_file_w_error.txt
	                                  --reference-file ref_file.txt
	                                  --num-expected-col 200 )
## The test above shall return a message to the user saying that he must specify the columns to compare:
set_tests_properties(Test_Compare_Files_No_Columns
                     PROPERTIES PASS_REGULAR_EXPRESSION
                     "Please specify which columns to compare using the option -C")
#########################











