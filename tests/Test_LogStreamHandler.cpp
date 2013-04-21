

#include "LogStreamHandler.h"


bool test_logfile_in_home_dir()
{
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
      return false;
   }
   else
   {
     std::cout << "Success: re-read test string from log file: " << lsh.fileName()
               << "\nString that was written: \n\"" << log_string << "\"\nString retrieved: \n\"" << result << '\"'<<std::endl;
     return true;
   }
}

int Test_LogStreamHandler(int, char*[] )
{
   int ret_ok=0, ret_fail=1;
   
   // Re-direct the clog stream to log file in home:
   if(!test_logfile_in_home_dir())
      return ret_fail;
   
   // Unset the "HOME" variable (this is used internally in
   // redirect_clog_toFileInHomeDir). This should provoke 
   
   int tmp = unsetenv("HOME");
   
   if(!test_logfile_in_home_dir())
      return ret_fail;
   
   return ret_ok;
}

