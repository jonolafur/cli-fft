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
void data_columns::zeroPadSamplesByFactorTwo()
{
	m_data[1].resize(m_data[1].size()*2, 0.0);
	m_data[2].resize(m_data[2].size()*2, 0.0);
}
///////////////////////////////////////////////////////////////////////////////
bool data_columns::checkEqualNonZeroColumnLength()
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
void getColsFromFile(StreamHandler& stream_handler, data_columns& columns,
		const std::vector<int>& columnIdx, std::string delimiter)
{
	if(columnIdx.empty())
		return;

	std::string rawLineFromFile;
	std::vector<std::string> tokenizedLine;
	int numberOfReportedErrors = 0;

	std::size_t largestColumnIndex = *(std::max_element(columnIdx.begin(), columnIdx.end()));

	while(stream_handler.getline(rawLineFromFile))
	{
		tokenize(tokenizedLine, rawLineFromFile, delimiter);

		// Only parse lines that have at least as many fields as the largest index:
		if(tokenizedLine.size()<largestColumnIndex) continue;

		for(std::size_t i=0; i<columnIdx.size(); i++)
		{
			if(columnIdx[i]<0) continue;
			try
			{
				ba::trim(tokenizedLine[columnIdx[i]]);

				columns.m_data[i].push_back( boost::lexical_cast<double>( tokenizedLine[columnIdx[i]] ) );
			}
			catch(boost::bad_lexical_cast& e)
			{
				numberOfReportedErrors++;
				if(numberOfReportedErrors < maxNumberOfFailuresToLog)
				{
					std::clog << "Failed to interpret field nr.: " << columnIdx[i]
				          << " at line nr.: " << stream_handler.lineCount()
				          << " in file: " << stream_handler.filename() << '\n';
				}
				if(numberOfReportedErrors == maxNumberOfFailuresToLog)
				{
					std::clog << "Log file flooding prevention: Already reported " << numberOfReportedErrors <<
					" bad fields. Will proceed quietly from now on. Note that more errors may occur during further file read...\n";
				}
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
///////////////////////////////////////////////////////////////////////////////











