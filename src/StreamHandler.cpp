/*
 * StreamHandler.cpp
 *
 *  Created on: Apr 14, 2013
 *      Author: jon
 */

#include "StreamHandler.h"

StreamHandler::StreamHandler() : m_in_stream(NULL), m_in_file()
{}

StreamHandler::~StreamHandler()
{
	m_in_file.close();
	m_in_stream=NULL;
}

void StreamHandler::init(std::string fileName)
{
	// Check and re-direct the input source:
	if(fileName.empty())
		m_in_stream = &std::cin;
	else
	{
		m_in_file.open( fileName.c_str() );
		if(!m_in_file.good())
			throw "Failed to open input file: "+fileName+"\n";
		m_in_stream = &m_in_file;
	}
}

bool StreamHandler::getline(std::string& rawLine)
{
	return std::getline(*m_in_stream, rawLine);
}

std::string StreamHandler::filename()
{
	if(m_fileName.empty())
		return "Standard input";
	return m_fileName;
}

