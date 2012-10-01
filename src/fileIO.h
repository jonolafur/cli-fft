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
static const int maxNumberOfFailuresToLog = 100;

class data_columns
{
public:
	std::vector<std::vector<double> > m_data;
	data_columns(): m_data(3){}
	virtual ~data_columns(){}

	std::vector<double>& x_axis(){return m_data[0];}
	std::vector<double>& realAxis(){return m_data[1];}
	std::vector<double>& imagAxis(){return m_data[2];}

	std::size_t size(){return m_data.size();}
	std::size_t max_size();
	bool checkEqualNonZeroColumnLength();
	void zeroPadSamplesByFactorTwo();
};

bool isCommentedOut(std::string line, std::string commentIdentifier="#");

void getColsFromFile(std::string fileName,
		data_columns& columns, const std::vector<int>& columnIdx,
		std::string delimiter);

void tokenize(std::vector<std::string>& result, const std::string& inputLine,
              const std::string& delimiter);

#endif /* FILEIO_H_ */



