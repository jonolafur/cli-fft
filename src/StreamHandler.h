/*
 * StreamHandler.h
 *
 *  Created on: Apr 14, 2013
 *      Author: jon
 */

#ifndef STREAMHANDLER_H_
#define STREAMHANDLER_H_

#include <iostream>
#include <fstream>
#include <string>

class StreamHandler {
private:
	std::istream* m_in_stream;
	std::ifstream m_in_file;
	std::string m_fileName;
	char m_commentChar;
	int m_lineCount, m_readLineCount;

public:
	StreamHandler();
	StreamHandler(const std::string& fileName, char commentChar='#');
	virtual ~StreamHandler();
	void init(const std::string& fileName, char commentChar='#');
	bool getline(std::string& rawLine);
	void setCommentChar(char commentChar){m_commentChar=commentChar;}
	std::string filename();
	template<typename T>
	StreamHandler& operator<< (const T& x)
	{
		(*m_in_stream) << x;
		return *this;
	}
	int lineCount(){return m_lineCount;}
	int readLineCount(){return m_readLineCount;}
	bool isCommentedOut(const std::string& line);

private:
	// Make non-copy-able:
	StreamHandler(StreamHandler& ): m_in_stream(NULL), m_in_file(), m_fileName(),
    m_commentChar('#'), m_lineCount(0), m_readLineCount(0) {}
	StreamHandler& operator=(const StreamHandler& ){return *this;}
};

#endif /* STREAMHANDLER_H_ */
