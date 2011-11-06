cmake_minimum_required (VERSION 2.6)


####################################################################################
### Adding sources #################################################################
set( BIN_NAME generator )
set( INST_DIR ~/bin )

add_executable (${BIN_NAME}
   generator.h
   generator.cpp)

# For the details on the names of the variables involving the
# finding and using of boost, refer to the FindBoost.cmake 
# module. The following tells the linker to link the
# program_options library file from boost (the program_options
# component from the boost package in cmake speak)

if(WIN32)
  target_link_libraries( ${BIN_NAME} )
else(WIN32)
  target_link_libraries( ${BIN_NAME} )
endif(WIN32)

install( TARGETS ${BIN_NAME} DESTINATION ${INST_DIR} )
message( STATUS "${BIN_NAME} will install to ${INST_DIR}" )


