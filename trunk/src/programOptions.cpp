/*
 * progamOptions.cpp
 *
 *  Created on: Oct 29, 2011
 *      Author: jon
 */

#include "programOptions.h"
#include "version.h"

///////////////////////////////////////////////////////////////////////////////
programOptions::programOptions() : var_map(),
     desc("Available options", getScreenWidth(), (getScreenWidth()*2)/3 )
{
	addProgramOptions();
}
///////////////////////////////////////////////////////////////////////////////
void programOptions::loadOptionsFromCommandLine(int argc, char* argv[])
{
	// store and notify:
	po::store(po::parse_command_line(argc, argv, desc), var_map);
	po::notify(var_map);
}
///////////////////////////////////////////////////////////////////////////////
int programOptions::getScreenWidth()
{
	int screen_col=tty_cols();
	// If the screen coordinates were available in the environment use it,
	// otherwise fall back to default
	if(screen_col <= 0)
		screen_col = po::options_description::m_default_line_length;

	return screen_col;
}
void programOptions::addProgramOptions()
{
	// declare all options:
	desc.add_options()
		("help,?", "Produce help message")
		("version,v", "Show the version of the program")
		("quiet,q", "Be less talkative");
}
///////////////////////////////////////////////////////////////////////////////
bool programOptions::writeHelpToConsole()
{
	if(var_map.empty() || var_map.count("help")!=0)
	{
		std::cout << desc << std::endl;
		return true;
	}

	return false;
}
///////////////////////////////////////////////////////////////////////////////
bool programOptions::writeVersionToConsole(const std::string& programName)
{
	if(var_map.count("version")>0)
	{
		std::cout << "This is " << programName << ", version: "
				<< ifen::version_string << std::endl
				<< "Built on: " << ifen::build_date_string << std::endl;
		return true;
	}
	return false;
}






