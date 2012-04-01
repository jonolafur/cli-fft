/*
 * fftOptions.cpp
 *
 *  Created on: Oct 29, 2011
 *      Author: jon
 */

#include "fftOptions.h"

///////////////////////////////////////////////////////////////////////////////
fftOptions::fftOptions(int argc, char* argv[]) :  programOptions()
{
	addfftOptions();
	loadOptionsFromCommandLine(argc, argv);
}
///////////////////////////////////////////////////////////////////////////////
void fftOptions::addfftOptions()
{
	// declare all options:
	desc.add_options()
	("input-file,i", po::value<std::string>(),
			"Name of input file. If nothing is specified, the standard input will be used.")
	("output-file,o", po::value<std::string>(),
			"Name of output file. If nothing is specified, the standard output will be used.")
	("delimiter,d", po::value<char>()->default_value(' '), "Delimiter for data fields.")
	("ordered-x-axis,O","The natural output of the fft is to have the positive frequencies "
			"from sample index 1 to N/2 and the negative frequencies from sample index N/2 to N. "
			"With this option the frequency axis is ordered, i.e. the smallest frequency is at "
			"sample index 1 and the largest frequency at sample index N")
	("positive-only,p", "If the input is real the spectrum is symmetric and only one half-axis carries "
			"information. This option suppresses the negative frequencies in the output if the input data is real.")
	("magnitude-phase,M","Output format: Writes the magnitude and phase to the output, "
			"rather than real and imaginary components.")
	("inverse,I", "Perform an inverse fft rather than a forward FFT.")
	("auto-correlate,A", "Perform an auto-correlation of the samples. The operation basically does IFFT(FFT*conj(FFT)).")
	("complex,c","Perform transformations on complex data. See -y option below.")
	("x-values,x", po::value<int>()->default_value(0),
			"x-values: fft will infer the frequency from the first two elements. "
			"If a non-uniformity is detected, fft will exit. The column numbering is 1-based."
			"If x-values is zero, no x-axis is assumed.")
	("y-values,y", po::value<int>()->default_value(1),
			"y-values: This column will be used for the real values of the samples to transform. "
			"If \"complex\" values are specified, the following column (y+1) will be used "
			"for the imaginary part.")
	("normalize,N", "Per default fftw does not normalize the output of the fft, that is if an FFT with "
			"subsequent IFFT is performed, the result will be scaled by a factor of N, where N "
			"is the number of samples. With this option the transformed result (FFT or IFFT) "
			"is normalized by sqrt(N), thus ensuring that after an FFT-IFFT cycle the "
                        "identical result is obtained");
}
///////////////////////////////////////////////////////////////////////////////
std::vector<int> fftOptions::getZeroBasedColumnIndexes()
{
	std::vector<int> colIdx;

	if(x_value_Idx()>0)
		colIdx.push_back(x_value_Idx()-1);

	if(y_value_Idx()>0)
	{
		colIdx.push_back(y_value_Idx()-1);

		if(isComplex())
			colIdx.push_back(y_value_Idx());
	}

	return colIdx;
}
///////////////////////////////////////////////////////////////////////////////
std::string fftOptions::getDelimiterAsString()
{
	return std::string(1,var_map["delimiter"].as<char>());
}
///////////////////////////////////////////////////////////////////////////////
std::string fftOptions::inputFile()
{
	if(var_map.count("input-file") ==0)
		return std::string();

	return var_map["input-file"].as<std::string>();
}
///////////////////////////////////////////////////////////////////////////////
std::string fftOptions::outputFile()
{
	if(var_map.count("output-file") ==0)
		return std::string();

	return var_map["output-file"].as<std::string>();
}


