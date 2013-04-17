/*
 * LogStreamHandler.h
 *
 *  Created on: Apr 14, 2013
 *      Author: jon
 */

#ifndef LOGSTREAMHANDLER_H_
#define LOGSTREAMHANDLER_H_

//////////////     Include section     ////////////////////////////////////////
// STL headers:
#include <string>
#include <iostream>
#include <iomanip>

class LogStreamHandler {
private:
   std::string      log_file_name;
   std::ofstream    log_file;
   std::streambuf*  backup;

public:
	LogStreamHandler();
	virtual ~LogStreamHandler();
   
   void redirect_clog( std::string log_path, std::ofstream& log_file );
   voi makeLogPath( std::string log_file_base_name );

};

#endif /* LOGSTREAMHANDLER_H_ */
