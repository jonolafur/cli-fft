/*
 * Test_get_field.cpp
 *
 *  Created on: Oct 30, 2011
 *      Author: jon
 */

#include <iostream>
#include <cmath>
#include "fileIO.h"

///////////////////////////////////////////////////////////////////////////////
void generateFile(std::string fileName, int numLines)
{
	std::cout << "Generating data file: " << fileName
			<< " with " << numLines<< " entries." << std::endl;

	std::ofstream out( fileName.c_str() );
	out.precision(12);

	out << "# Starting off with a comment\n";

	double t=0.0;
	double w = 2.0*3.141567;
	for(int i=0; i<numLines; i++, t+=0.011)
	{
		out << t << ' '
	          << 0.1 + cos(w*t) +2.3*sin(2*w*t) << ' '
	          << 0.1 + sin(w*t) +2.3*cos(2*w*t) << ' '
	          << cos(w*t*t) << ' ' << sin(w*t*t*2.0) << std::endl;
	}

	out << "# Finishing off with a comment\n";

	out << "\n\n";  // Trailing new lines...

	out.close();
	std::cout << "Finished generating data file." << std::endl;
}
///////////////////////////////////////////////////////////////////////////////
bool readFile( std::string fileName, std::vector<int> columnIdx, std::size_t numberOfLines )
{
	data_columns columns;
	std::string delimiter = " ";

	getColsFromFile(fileName, columns, columnIdx, delimiter);

	if(columns.size() != columnIdx.size())
	{
		std::cout << "Unable to retrieve the required number of columns.\n";
		return false;
	}

	if(!columns.checkEqualNonZeroColumnLength())
	{
		std::cout << "Unequal columns detected.\n";
		return false;
	}

	if(columns.max_size() != numberOfLines)
	{
		std::cout << "Incorrect number of entries read. Should have been: "
				<< numberOfLines << " but " << columns.max_size() << " were retrieved.\n";
		return false;
	}

	for(std::size_t i = 0; i<columns.size(); i++)
	{
		if(columnIdx[i]<0 && columns.m_data[i].size()!=0)
		{
			std::cout << "Index for column nr. " << i
					<< " was negative, but the corresponding column contains "
					<< columns.m_data[i].size() << " entries, should have been zero.\n";
			return false;
		}
	}

	return true;
}
///////////////////////////////////////////////////////////////////////////////
bool checkColumnLength(std::string fileName, std::size_t numberOfLines )
{
	data_columns columns;
	std::string delimiter = " ";
	std::vector<int> columnIdx;

	columnIdx.push_back(1);
	columnIdx.push_back(2);
	columnIdx.push_back(3);

	getColsFromFile(fileName, columns, columnIdx, delimiter);

	// Change the size of one of the non-zero length cols:
	columns.m_data[0].resize(numberOfLines*2);

	if(!columns.checkEqualNonZeroColumnLength())
	{
		std::cout << "Unequal columns correctly detected.\n";
		return true;
	}

	std::cout << "Unequal columns should have been detected.\n";
	return false;
}
///////////////////////////////////////////////////////////////////////////////
int Test_read_file(int, char*[] )
{
	int ret_ok=0, ret_fail=1;
	std::size_t numberOfLines = 2000;

	std::vector<int> columnIdx;
	std::string delimiter = " ";
	std::string fileName("Test_read_file.txt");

	columnIdx.push_back(1);
	columnIdx.push_back(2);
	columnIdx.push_back(3);

	generateFile(fileName, numberOfLines);

	if( !readFile(fileName,columnIdx,numberOfLines) )
		return ret_fail;

	columnIdx[0] = -1;

	if( !readFile(fileName,columnIdx,numberOfLines) )
		return ret_fail;

	columnIdx[2] = -1;

	if( !readFile(fileName,columnIdx,numberOfLines) )
		return ret_fail;

	if( !checkColumnLength(fileName, numberOfLines) )
		return ret_fail;

	return ret_ok;
}


