/*
 * fileIO.cpp
 *
 *  Created on: Oct 30, 2011
 *      Author: jon
 */

#include "fileIO.h"

typedef boost::char_separator<char> token_separator;
typedef boost::tokenizer<boost::char_separator<char> > tokenizer;

///////////////////////////////////////////////////////////////////////////////
bool isCommentedOut(std::string line, std::string commentIdentifier)
{
	if(line.empty())
		return true;

	return (line[0]==commentIdentifier[0]);
}
///////////////////////////////////////////////////////////////////////////////
void tokenizeString(std::vector<std::string>& tokenVector,
		std::string& s, std::string delimiter)
{
	tokenVector.clear();
	token_separator sep;
	if( !delimiter.empty() )
		sep = token_separator(delimiter.c_str());

	tokenizer tokens(s, sep);
	for(tokenizer::iterator tok_iter = tokens.begin(); tok_iter != tokens.end(); ++tok_iter)
		tokenVector.push_back( std::string(*tok_iter) );
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

		tokenizeString(tokenizedLine, rawLineFromFile, delimiter);

		if(tokenizedLine.size()<largestColumnIndex) continue;

		for(std::size_t i=0; i<columnIdx.size(); i++)
		{
			double fieldValue = boost::lexical_cast<double>( tokenizedLine[columnIdx[i]]);
			columns[i].push_back( fieldValue );
		}
	}
}












