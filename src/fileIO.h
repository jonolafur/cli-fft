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

#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>

///////////////////////////////////////////////////////////////////////////////
typedef std::vector<std::vector<double> > data_columns;

bool isCommentedOut(std::string line, std::string commentIdentifier="#");

void tokenizeString(std::vector<std::string>& tokenVector,
		std::string& s, std::string delimiter="");

void getColsFromFile(std::string fileName,
		data_columns& columns, std::vector<int>& columnIdx,
		std::string delimiter);


#endif /* FILEIO_H_ */



