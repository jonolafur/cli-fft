/*
 * fileIO.h
 *
 *  Created on: Oct 30, 2011
 *      Author: jon
 */

#ifndef FILE_IO_H_
#define FILE_IO_H_

#include <vector>
#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <locale>

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

#include "StreamHandler.h"

namespace ba = boost::algorithm;

using std::vector;
using std::string;

///////////////////////////////////////////////////////////////////////////////
static const int maxNumberOfFailuresToLog = 100;

class data_columns
{
public:
   vector<std::vector<double> > m_data;
   data_columns(): m_data(3){}
   virtual ~data_columns(){}

   vector<double>& x_axis(){return m_data[0];}
   vector<double>& realAxis(){return m_data[1];}
   vector<double>& imagAxis(){return m_data[2];}

   std::size_t size(){return m_data.size();}
   std::size_t max_size();
   bool checkEqualNonZeroColumnLength();
   void zeroPadSamplesByFactorTwo();
};

void getColsFromFile(StreamHandler& stream_handler,
                     data_columns& columns, const vector<int>& columnIdx,
                     string delimiter);

void tokenize(vector<string>& result, const string& inputLine,
              const string& delimiter);

#endif /* FILEIO_H_ */





