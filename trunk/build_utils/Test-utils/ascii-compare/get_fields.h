///////////////////////////////////////////////////////////////////////////////
//                              IfEN GmbH                                    //
///////////////////////////////////////////////////////////////////////////////
// Description:
// Little template helper function to pick out the n-th fields from a string.
// Fields are separated with white-space(s) or delim
// Project: NA
///////////////////////////////////////////////////////////////////////////////
// SVN Header (The following information will be automatically updated by SVN)
// $Author: jon $
// $Date: 2010-02-07 11:43:45 +0100 (Sun, 07 Feb 2010) $
// $Revision: 343 $
// $State: Exp $
// $Id: main.cpp 343 2010-02-07 10:43:45Z jon $
///////////////////////////////////////////////////////////////////////////////
// Include guards:
#ifndef _GET_FIELDS_H
#define _GET_FIELDS_H

//////////////     Include section     ////////////////////////////////////////
// STL headers:
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <algorithm>

///////////////////////////////////////////////////////////////////////////////
// Declarations: ////////////////////////////////////////////////////////////
template <typename T>
std::vector<T> get_fields(std::string& str, std::vector<int>& n_vec, char delim=' ')
{
	T x;
	std::vector<T> x_vec;
	std::stringstream sstr(str);
	std::string tmp;

	if(n_vec.size()==0)
		return x_vec;

	std::sort(n_vec.begin(), n_vec.end());

	for(int i=0; i<=n_vec.back(); ++i)
	{
		if(delim != ' ')			// If the data is delimited with a non-white space
			std::getline(sstr, tmp, delim);
		else
			sstr >> tmp;
		if(std::includes(n_vec.begin(), n_vec.end(), &i, &i+1) )
		{
			std::stringstream formater(tmp);
			formater >> x;
			x_vec.push_back(x);
		}
	}
	if(n_vec.size() != x_vec.size() )
	{
		std::string s("Failed to read all indexes in get_fields.");
		throw s;
	}
	return x_vec;
}

#endif // _GET_FIELDS_H





