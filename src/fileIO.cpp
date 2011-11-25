/*
 * fileIO.cpp
 *
 *  Created on: Oct 30, 2011
 *      Author: jon
 */

#include "fileIO.h"

///////////////////////////////////////////////////////////////////////////////
bool isCommentedOut(std::string line, std::string commentIdentifier)
{
	if(line.empty())
		return true;

	return (line[0]==commentIdentifier[0]);
}
///////////////////////////////////////////////////////////////////////////////
void getColsFromFile(std::string fileName, data_columns& columns,
		std::vector<int>& columnIdx, std::string delimiter)
{
	if(columnIdx.empty())
		return;

	std::string rawLineFromFile;
	std::vector<std::string> tokenizedLine;
	std::istream* in_stream;
	std::ifstream in_file;

	if(fileName.empty())
		in_stream = &std::cin;
	else
	{
		in_file.open( fileName.c_str() );
		in_stream = &in_file;
	}

	columns.resize(columnIdx.size());

	std::size_t largestColumnIndex = *(std::max_element(columnIdx.begin(), columnIdx.end()));

	while(getline(*in_stream, rawLineFromFile))
	{
		if( isCommentedOut(rawLineFromFile) ) continue;

		tokenize(tokenizedLine, rawLineFromFile, delimiter);
		if(tokenizedLine.size()<largestColumnIndex) continue;

		for(std::size_t i=0; i<columnIdx.size(); i++)
		{
			double fieldValue = boost::lexical_cast<double>( tokenizedLine[columnIdx[i]]);
			columns[i].push_back( fieldValue );
		}
	}
}
///////////////////////////////////////////////////////////////////////////////
void tokenize(std::vector<std::string>& result, const std::string& inputLine,
              const std::string& delimiter)
{
	if(delimiter.empty() || delimiter==" ")
		ba::split(result, inputLine, isspace, ba::token_compress_on);
	else
		ba::split(result, inputLine, ba::is_any_of(delimiter), ba::token_compress_on);
}











