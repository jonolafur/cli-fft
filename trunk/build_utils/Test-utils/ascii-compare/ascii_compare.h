///////////////////////////////////////////////////////////////////////////////
//                              IfEN GmbH                                    //
///////////////////////////////////////////////////////////////////////////////
// Description:
//   A tiny file compare utility to be used with ctest. It is intended for
//   comapring ASCII files containing columns of numbers
// Dependencies (others than those explicitly visible in the #include clause):
//    None.
// Project: NA
///////////////////////////////////////////////////////////////////////////////
// SVN Header (The following information will be automatically updated by SVN)
// $Author: jon $
// $Date: 2010-02-07 11:43:45 +0100 (Sun, 07 Feb 2010) $
// $Revision: 343 $
// $State: Exp $
// $Id: main.cpp 343 2010-02-07 10:43:45Z jon $
///////////////////////////////////////////////////////////////////////////////
// Include guards:
#ifndef _RX_MAIN_H
#define _RX_MAIN_H
//////////////     Include section     ////////////////////////////////////////
// STL headers:
#include <cmath>
#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>

// Boost headers:
#include <boost/program_options.hpp>

#include "version.h"
#include "get_fields.h"
///////////////////////////////////////////////////////////////////////////////
// short hand for program option name space...
namespace po = boost::program_options;

struct config
{
	std::string test_file_name, ref_file_name;
	std::vector<double> tol_deviation, tol_max_deviation;
	std::vector<double> deviation, max_deviation;
	std::vector<int> columns, max_dev_index;
	char comment;
	char delimiter;
	int num_compared_lines, num_expected_lines;
};

///////////////////////////////////////////////////////////////////////////////
// Function declarations...
int main(int argc, char* argv[]);
void compareResults(config& cfg);
void compareFiles(config& cfg);
std::string getNextLine(std::ifstream& in, char comment);
void checkStream(std::ifstream& in);


#endif //_RX_MAIN_H


