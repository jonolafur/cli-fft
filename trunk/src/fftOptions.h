/*
 * fftOptions.h
 *
 *  Created on: Oct 29, 2011
 *      Author: jon
 */

#ifndef FFTOPTIONS_H_
#define FFTOPTIONS_H_

#include "programOptions.h"

class fftOptions : public programOptions
{
public:
	std::string inFileName, outFileName;
	int xColumn;
	int yColumn;
	char delimiter;

public:
	fftOptions();
	virtual ~fftOptions(){};

	std::vector<int> getZeroBasedColumnIndexes();
	std::string getDelimiterAsString();
	bool isComplex(){return var_map.count("complex");}
	bool inverseFFT(){return var_map.count("inverse");}
	bool xAxisSpecified(){return xColumn!=0;}
	bool orderSamples(){return (var_map.count("ordered-x-axis") && !positiveAxisOnly() );}
	bool positiveAxisOnly(){return ( var_map.count("positive-only") && !isComplex() );}
	bool writeMagnitudeAndPhase(){return var_map.count("magnitude-phase");}

protected:
	void addfftOptions();

};



#endif /* FFTOPTIONS_H_ */

