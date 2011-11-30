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
	fftOptions();
	virtual ~fftOptions(){};

	std::vector<int> getZeroBasedColumnIndexes();
	std::string getDelimiterAsString();
	bool isComplex() const {return var_map.count("complex");}
	bool xAxisSpecified() const {return xColumn!=0;}
	bool orderSamples() const {return (var_map.count("ordered-x-axis") && !positiveAxisOnly() );}
	bool positiveAxisOnly() const {return ( var_map.count("positive-only") && !isComplex() );}
	bool normalize() const {return var_map.count("normalize");}

	bool writeMagnitudeAndPhase() const {return magnitude_phase;}
	bool inverseFFT() const {return inverse;}

protected:
	void addfftOptions();

};



#endif /* FFTOPTIONS_H_ */

