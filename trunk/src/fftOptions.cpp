/*
 * fftOptions.cpp
 *
 *  Created on: Oct 29, 2011
 *      Author: jon
 */

#include "fftOptions.h"

///////////////////////////////////////////////////////////////////////////////
fftOptions::fftOptions(int argc, char* argv[]) :  programOptions(),
   inFileName(), outFileName(), xColumn(0), yColumn(1), delimiter(' '),
   magnitude_phase(false), inverse(false)
{
	addfftOptions();
	loadOptionsFromCommandLine(argc, argv);

	magnitude_phase = var_map.count("magnitude-phase") !=0;
	inverse         = var_map.count("inverse") !=0;
}
///////////////////////////////////////////////////////////////////////////////
void fftOptions::addfftOptions()
{
	// declare all options:
	desc.add_options()
	("input-file,i", po::value<std::string>(&inFileName),
			"Name of input file. If nothing is specified, the standard input will be used.")
	("output-file,o", po::value<std::string>(&outFileName)->default_value(outFileName),
			"Name of output file. If nothing is specified, the standard output will be used.")
	("delimiter,d", po::value<char>(&delimiter)->default_value(delimiter), "Delimiter for data fields.")
	("ordered-x-axis,O","The natural output of the fft is to have the positive frequencies "
			"from sample index 1 to N/2 and the negative frequencies from sample index N/2 to N. "
			"With this option the frequency axis is ordered, i.e. the smallest frequency is at "
			"sample index 1 and the largest frequency at sample index N")
	("positive-only,p", "If the input is real the spectrum is symmetric and only one half-axis carries "
			"information. This option suppresses the negative frequencies in the output if the input data is real.")
	("magnitude-phase,M","Output format: Writes the magnitude and phase to the output, "
			"rather than real and imaginary components.")
	("inverse,I", "Perform an inverse fft rather than a forward FFT.")
	("complex,c","Perform transformations on complex data.")
	("x-values,x", po::value<int>(&xColumn)->default_value(xColumn),
			"x-values: fft will infer the frequency from the first two elements. "
			"If a non-uniformity is detected, fft will exit. The column numbering is 1-based."
			"If x-values is zero, no x-axis is assumed.")
	("y-values,y", po::value<int>(&yColumn)->default_value(yColumn),
			"y-values: This column will be used for the real values of the samples to transform. "
			"If \"complex\" values are specified, the following column (y+1) will be used "
			"for the imaginary part.")
	("normalize,N", "Per default fftw does not normalize the output of the fft, that is if an FFT with "
			"subsequent IFFT is performed, the result will be scaled by a factor of N, where N "
			"is the number of samples. With this option the transformed result (FFT or IFFT) "
			"is scaled by sqrt(N).");
}
///////////////////////////////////////////////////////////////////////////////
std::vector<int> fftOptions::getZeroBasedColumnIndexes()
{
	std::vector<int> colIdx;

	if(xColumn>0)
		colIdx.push_back(xColumn-1);

	if(yColumn>0)
	{
		colIdx.push_back(yColumn-1);

		if(isComplex())
			colIdx.push_back(yColumn);
	}

	return colIdx;
}
///////////////////////////////////////////////////////////////////////////////
std::string fftOptions::getDelimiterAsString()
{
	std::string tmp;
	tmp = delimiter;
	return tmp;
}


