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
public:
	StreamHandler();
	virtual ~StreamHandler();
	void init(std::string fileName);
	bool getline(std::string& rawLine);
	std::string filename();
	template<typename T>
	StreamHandler& operator<< (const T& x)
	{
		(*m_in_stream) << x;
		return *this;
	}
private:
	// Make non-copy-able:
	StreamHandler(StreamHandler& ): m_in_stream(NULL), m_in_file(), m_fileName(){};
	StreamHandler& operator=(const StreamHandler& ){return *this;}
};

#endif /* STREAMHANDLER_H_ */
