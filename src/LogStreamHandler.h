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
#include <fstream>
#include <cstdlib>

class LogStreamHandler {
private:
   std::string      log_file_path;
   std::string      log_file_base_name;
   std::ofstream    log_file;
   std::streambuf*  clog_buffer;

public:
	LogStreamHandler();
	virtual ~LogStreamHandler();
   
   std::string fileName(){return log_file_path;}
   void redirect_clog_toFileInHomeDir( const std::string& fileName );

protected:
   void redirect_clog(  );
   void makeLogPath(  );

private:
   // Make non-copy-able:
   LogStreamHandler(LogStreamHandler& ): log_file_path(), log_file_base_name(), log_file(), clog_buffer(nullptr){};
   LogStreamHandler& operator=(const LogStreamHandler& ){return *this;}

};

#endif /* LOGSTREAMHANDLER_H_ */
