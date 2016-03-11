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
	fftOptions(int argc, char* argv[]);
	virtual ~fftOptions(){};

	std::vector<int> getZeroBasedColumnIndexes();
	std::string getDelimiterAsString();
	bool isComplex() const {return var_map.count("complex")!=0;}
	std::string inputFile();
	std::string outputFile();
	char delimiter(){return var_map["delimiter"].as<char>();}
   bool sampleWasRateSpecified() const {return var_map.count("sample-rate")!=0 ;}
   double getSampleRate() const {return var_map["sample-rate"].as<double>();}
	bool xAxisSpecified() const {return x_value_Idx()!=0;}
	bool orderSamples() const {return (var_map.count("ordered-x-axis")!=0 && !positiveAxisOnly() );}
	bool positiveAxisOnly() const {return ( var_map.count("positive-only")!=0 && !isComplex() );}
	bool normalize() const {return var_map.count("normalize")!=0;}
	bool acf() const {return var_map.count("auto-correlate") !=0;}
	bool estimate_fftw() const {return var_map.count("estimate-fftw") !=0;}
	bool cyclicAcf() const;
	bool zeroPadSamples() const;
	bool removeBartlett() const;
	bool writeMagnitudeAndPhase() const {return var_map.count("magnitude-phase") !=0;}
	bool inverseFFT() const {return var_map.count("inverse") !=0;}
	size_t getNumBatches() const;


protected:
	void addfftOptions();

private:
	int x_value_Idx() const { return var_map["x-values"].as<int>(); };
	int y_value_Idx() const { return var_map["y-values"].as<int>(); };

};



#endif /* FFTOPTIONS_H_ */

