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
	bool magnitude_phase;
	bool inverse;

public:
	fftOptions(int argc, char* argv[]);
	virtual ~fftOptions(){};

	std::vector<int> getZeroBasedColumnIndexes();
	std::string getDelimiterAsString();
	bool isComplex() const {return var_map.count("complex")!=0;}
	bool xAxisSpecified() const {return xColumn!=0;}
	bool orderSamples() const {return (var_map.count("ordered-x-axis")!=0 && !positiveAxisOnly() );}
	bool positiveAxisOnly() const {return ( var_map.count("positive-only")!=0 && !isComplex() );}
	bool normalize() const {return var_map.count("normalize")!=0;}

	bool writeMagnitudeAndPhase() const {return magnitude_phase;}
	bool inverseFFT() const {return inverse;}

protected:
	void addfftOptions();

};



#endif /* FFTOPTIONS_H_ */

