/*
 * LogStreamHandler.cpp
 *
 *  Created on: Apr 14, 2013
 *      Author: jon
 */

#include "LogStreamHandler.h"

LogStreamHandler::LogStreamHandler(): log_file_path(), log_file_base_name(), log_file(), clog_buffer(nullptr)
{
}

LogStreamHandler::~LogStreamHandler()
{
   if(clog_buffer!=nullptr)
      std::clog.rdbuf(clog_buffer);
}

void LogStreamHandler::redirect_clog_toFileInHomeDir(const std::string& fileName)
{
   log_file_base_name = fileName;
   makeLogPath();
   redirect_clog();
}

///////////////////////////////////////////////////////////////////////////////
void LogStreamHandler::makeLogPath()
{
   char* homePath = getenv("HOME");

   if(homePath)
   {
      log_file_path = std::string( homePath ) + "/" + log_file_base_name;
      return;
   }
   else
   {
      homePath = getenv("HOMEPATH");
      char* homeDrive = getenv("HOMEDRIVE");
      
      if(homePath && homeDrive)
      {
         log_file_path = std::string( homeDrive )+std::string( homePath ) + "/" + log_file_base_name;
         return;
      }
   }

   log_file_path = log_file_base_name;
   std::cout << "# Unable to retrieve home path. Writing log to current directory.\n";
}

///////////////////////////////////////////////////////////////////////////////
void LogStreamHandler::redirect_clog()
{
   clog_buffer = std::clog.rdbuf();

   log_file.open(log_file_path.c_str());

   if(log_file)
      std::clog.rdbuf( log_file.rdbuf() );
   else
   {
      std::string s = "Failed to open file: " + log_file_path + "for writing.\n";
      throw s;
   }

}


