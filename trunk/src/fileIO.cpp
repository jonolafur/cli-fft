/*
 * fileIO.cpp
 *
 *  Created on: Oct 30, 2011
 *      Author: jon
 */

#include "fileIO.h"

///////////////////////////////////////////////////////////////////////////////
std::size_t data_columns::max_size()
{
	std::size_t N = m_data[0].size();

	for(std::size_t i=1; i<size(); i++)
	{
		if(N<m_data[i].size())
			N=m_data[i].size();
	}
	return N;
}
///////////////////////////////////////////////////////////////////////////////
bool data_columns::checkEqualNonzeroColumnLength()
{
	std::size_t N = max_size();

	for(std::size_t i=0; i<size(); i++)
	{
		if(m_data[i].size() != N && m_data[i].size()!=0)
		{
			std::clog << "Unequal, non-zero column sizes." << std::endl;
			return false;
		}
	}
	return true;
}
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
	int line_count =0;

	// Check and re-direct the input source:
	if(fileName.empty())
		in_stream = &std::cin;
	else
	{
		in_file.open( fileName.c_str() );
		if(!in_file.good())
			throw "Failed to open input file: "+fileName+"\n";
		in_stream = &in_file;
	}

	std::size_t largestColumnIndex = *(std::max_element(columnIdx.begin(), columnIdx.end()));

	while(getline(*in_stream, rawLineFromFile))
	{
		line_count++;
		if( isCommentedOut(rawLineFromFile) ) continue;

		tokenize(tokenizedLine, rawLineFromFile, delimiter);

		// Only parse lines that have at least as many fields as the largest index:
		if(tokenizedLine.size()<largestColumnIndex) continue;

		for(std::size_t i=0; i<columnIdx.size(); i++)
		{
			if(columnIdx[i]<0) continue;
			try
			{
				double fieldValue = boost::lexical_cast<double>( tokenizedLine[columnIdx[i]]);
				columns.m_data[i].push_back( fieldValue );
			}
			catch(boost::bad_lexical_cast& e)
			{
				std::clog << "Failed to interpret field nr.: " << columnIdx[i]
				          << " at line nr.: " << line_count << " in file: " << fileName << '\n';
			}
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











