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

namespace ba = boost::algorithm;

///////////////////////////////////////////////////////////////////////////////
typedef std::vector<std::vector<double> > data_columns;

bool isCommentedOut(std::string line, std::string commentIdentifier="#");

void getColsFromFile(std::string fileName,
		data_columns& columns, std::vector<int>& columnIdx,
		std::string delimiter);
void tokenize(std::vector<std::string>& result, const std::string& inputLine,
              const std::string& delimiter);

#endif /* FILEIO_H_ */



