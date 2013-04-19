

#include "LogStreamHandler.h"


int Test_LogStreamHandler(int, char*[] )
{
   int ret_ok=0, ret_fail=1;

   LogStreamHandler lsh;
   std::string log_file_name(".test_logStream_handler.log");
   std::string log_string("This is a test string.");
   std::string result;
   
   
   lsh.redirect_clog_toFileInHomeDir(log_file_name);
   
   std::clog << log_string << std::flush;
   
   std::ifstream log_file(lsh.fileName().c_str());
   
   std::getline(log_file, result);
   
   if(result != log_string)
   {
      std::cout << "Failed to re-read test string from log file: " << lsh.fileName()
               << "\nString that was written: \n\"" << log_string << "\"\nString retrieved: " << result << std::endl;
      return ret_fail;
   }
   else
   {
     std::cout << "Success: re-read test string from log file: " << lsh.fileName()
               << "\nString that was written: \n\"" << log_string << "\"\nString retrieved: \n\"" << result << '\"'<<std::endl;
     return ret_ok;
   }
}

