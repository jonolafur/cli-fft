/*
 * LogStreamHandler.cpp
 *
 *  Created on: Apr 14, 2013
 *      Author: jon
 */

#include "LogStreamHandler.h"

LogStreamHandler::LogStreamHandler(): log_file_name(".fft.log"), log_file(), backup(NULL)
{
   log_file_name = makeLogPath(log_file_name);
   backup = redirect_clog(log_file_name, log_file );
}

LogStreamHandler::~LogStreamHandler()
{
   std::clog.rdbuf(backup);
}

///////////////////////////////////////////////////////////////////////////////
void LogStreamHandler::redirect_clog(std::string log_path,
                              std::ofstream& log_file)
{
   std::streambuf* clog_buffer = std::clog.rdbuf();

   log_file.open(log_path.c_str());

   if(log_file)
      std::clog.rdbuf( log_file.rdbuf() );
   else
   {
      std::string s = "Failed to open file: " + log_path + "for writing.\n";
      throw s;
   }

   return clog_buffer;
}
///////////////////////////////////////////////////////////////////////////////
voi LogStreamHandler::makeLogPath(std::string log_file_base_name)
{
   bool readHomePathOk=false;
   std::string log_path;

   char* homePath = getenv("HOME");

   if(homePath)
   {
      readHomePathOk=true;
      log_path = std::string( homePath ) + "/" + log_file_base_name;
   }
   else
      log_path = log_file_base_name;

   if(!readHomePathOk)
      std::cout << "# Unable to retrieve home path. Writing log to current directory.\n";

   return log_path;
}
