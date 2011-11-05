/*
 * programOptions.h
 *
 *  Created on: Oct 29, 2011
 *      Author: jon
 */

#ifndef PROGRAMOPTIONS_H_
#define PROGRAMOPTIONS_H_
// STL headers:
#include <string>
#include <iostream>
#include <iomanip>

// Boost headers:
#include <boost/program_options.hpp>

// Project headers:
#include "tty_cols.h"

///////////////////////////////////////////////////////////////////////////////
// short hand for program option name space...
namespace po = boost::program_options;

class programOptions
{
protected:
	po::variables_map var_map;
	po::options_description desc;

public:
	programOptions();
	virtual ~programOptions(){};
	void loadOptionsFromCommandLine(int argc, char* argv[]);

	bool quiet(){return var_map.count("quiet")!=0;}
	bool writeHelpToConsole();
	bool writeVersionToConsole(const std::string& programName);

protected:
	int getScreenWidth();
	void addProgramOptions();
};


#endif /* PROGRAMOPTIONS_H_ */
