/*
 * Test_get_field.cpp
 *
 *  Created on: Oct 30, 2011
 *      Author: jon
 */
#include <iostream>
#include "fileIO.h"

int ret_ok=0, ret_fail=1;

///////////////////////////////////////////////////////////////////////////////
void fillString(std::string& s, const std::vector<std::string>& tokens, std::string delimiter)
{
	s.clear();

	for(std::size_t i=0; i<tokens.size()-1; i++)
		s += (tokens[i]+ delimiter);
	s += tokens.back();
}
///////////////////////////////////////////////////////////////////////////////
bool compareExtractedTokens(std::string& s,
		const std::vector<std::string>& tokens, std::string delimiter)
{
	std::vector<std::string> extracted;
	tokenize(extracted, s, delimiter);

	if(tokens.size()!=extracted.size())
	{
		std::cout << "Error: Unexpected number of tokens extracted." << std::endl;
		return false;
	}

	for(std::size_t i=0; i<tokens.size(); ++i )
	{
		std::cout << "Token expected: <" << tokens[i] << ">. Token extracted: <"<< extracted[i] << '>' <<  std::endl;
		if(tokens[i] != extracted[i])
		{
			std::cout << "Error: token mismatch." << std::endl;
			return false;
		}
	}
	return true;
}
///////////////////////////////////////////////////////////////////////////////
int Test_get_field(int, char*[] )
{
	std::string delimiter("  \t \n \r  ");  // non-trivial white-space delimiter...
	std::string s;
	std::vector<std::string> tokens;
	tokens.push_back("hello");
	tokens.push_back("world");
	tokens.push_back("-.123");
	tokens.push_back("456e3");
	tokens.push_back("234.598e4");

	std::cout << std::endl << "Testing tokenizing using white space as delimiter..." << std::endl;
	fillString(s, tokens, delimiter);
	delimiter="";
	if(!compareExtractedTokens(s, tokens, delimiter))
		return ret_fail;

	delimiter = ",";
	std::cout << std::endl << "Testing tokenizing using \',\' as delimiter..." << std::endl;
	fillString(s, tokens, delimiter);
	if(!compareExtractedTokens(s,tokens, delimiter))
		return ret_fail;

	return ret_ok;
}


