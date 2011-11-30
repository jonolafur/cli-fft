/*
 * Test_get_field.cpp
 *
 *  Created on: Oct 30, 2011
 *      Author: jon
 */

#include <iostream>
#include <cmath>
#include "fileIO.h"

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
bool checkEqualColumnLength(const data_columns& columns)
{
	std::size_t N = columns[0].size();

	for(std::size_t i=1; i<columns.size(); i++)
	{
		if(columns[i].size() != N)
		{
			std::cout << "Unequal entries in column: " << i << " and " << i-1;
			return false;
		}
	}
	return true;
}
///////////////////////////////////////////////////////////////////////////////
int Test_read_file(int, char*[] )
{
	int ret_ok=0, ret_fail=1;
	std::size_t numberOfLines = 2000;

	data_columns columns;
	std::vector<int> columnIdx;
	std::string delimiter = " ";
	std::string fileName("Test_read_file.txt");

	columnIdx.push_back(1);
	columnIdx.push_back(2);
	columnIdx.push_back(3);


	generateFile(fileName, numberOfLines);

	getColsFromFile(fileName, columns, columnIdx, delimiter);

	if(columns.size() != columnIdx.size())
	{
		std::cout << "Unable to retrieve the required number of columns.\n";
		return ret_fail;
	}

	checkEqualColumnLength(columns);

	if(columns[0].size() != numberOfLines)
	{
		std::cout << "Incorrect number of entries read. Should have been: "
				<< numberOfLines << " but " << columns[0].size() << " were retrieved.\n";
		return ret_fail;
	}

	return ret_ok;
}


