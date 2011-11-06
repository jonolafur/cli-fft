///////////////////////////////////////////////////////////////////////////////
// Description:
//   Defines the entry point for the fft console application.
//   This is a very simple application that performs fft and ifft on data sets
//   in ASCII files and outputs on std out or into file
///////////////////////////////////////////////////////////////////////////////
// SVN Header (The following information will be automatically updated by SVN)
// $Author:  $
// $Date:  $
// $Revision:  $
// $Id:  $
///////////////////////////////////////////////////////////////////////////////
//Includes: ///////////////////////////////////////////////////////////////////
#include "main.h"
///////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
{
	const int ret_error = 1;
	const int ret_ok = 0;

	int ret_val = ret_ok;
	std::string programName("fft");
	
	try
	{
		fftOptions opt;
		opt.loadOptionsFromCommandLine(argc, argv);

		if(opt.writeVersionToConsole(programName))
			return ret_ok;

		if(opt.writeHelpToConsole())
			return ret_ok;

		fftw_vector fft_vec;

		readInput(opt, fft_vec);
		processInput(opt,fft_vec);
		writeOutput(opt,fft_vec);

	}
	catch(std::exception& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		ret_val = ret_error;
	}
	catch(std::string& s)
	{
		std::cerr << "Error: " << s << std::endl;
		ret_val = ret_error;
	}
	catch(const char* s)
	{
		std::cerr << "Error: " << s << std::endl;
		ret_val = ret_error;
	}
	catch(...)
	{
		std::cerr << "Exception of unknown type!\n";
		ret_val = ret_error;
	}

	return ret_val;
}
///////////////////////////////////////////////////////////////////////////////
void readInput(fftOptions& opt, fftw_vector& fft_vec)
{
	data_columns fft_data;

	std::vector<int> colIdx = opt.getZeroBasedColumnIndexes();

	getColsFromFile(opt.inFileName, fft_data, colIdx, opt.getDelimiterAsString() );

	if(opt.isComplex())
	{
		if( opt.xAxisSpecified())
			fft_vec.set_samples(fft_data[1], fft_data[2], false);
		else
			fft_vec.set_samples(fft_data[0], fft_data[1], false);
	}
	else
	{
		if(opt.xAxisSpecified())
			fft_vec.set_samples(fft_data[1], false);
		else
			fft_vec.set_samples(fft_data[0], false);
	}

	if(!opt.inverseFFT() && opt.xAxisSpecified())
	{
		double dt =  checkSampleTime(fft_data[0]);
		fft_vec.setSampleTime(dt);
	}
}
///////////////////////////////////////////////////////////////////////////////
void processInput(fftOptions& opt, fftw_vector& fft_vec)
{
	if(opt.inverseFFT())
		fft_vec.ifft();
	else
		fft_vec.fft();
}
///////////////////////////////////////////////////////////////////////////////
void writeOutput(fftOptions& opt, fftw_vector& fft_vec)
{
	std::ostream* out_stream;
	std::ofstream out_file;

	if(!opt.outFileName.empty())
	{
		out_file.open(opt.outFileName.c_str());
		out_stream = &out_file;
	}
	else
		out_stream = &std::cout;

	if(opt.orderSamples())
		writeOrdered(fft_vec,out_stream, opt.writeMagnitudeAndPhase());

	writeStandard(fft_vec, out_stream, opt.positiveAxisOnly(), opt.writeMagnitudeAndPhase() );

}
///////////////////////////////////////////////////////////////////////////////
double checkSampleTime(const std::vector<double>& t)
{
	std::size_t N = t.size();
	if(N<2) throw "Error: less than two samples in checkSampleTime.\n";

	double dt = ( t.back()-t.front() )/static_cast<double>(N-1);
	// TODO: generate some statistics on the time axis
	// (max/min deviation from average sample time etc.)
	return dt;
}
///////////////////////////////////////////////////////////////////////////////
void writeOrdered(fftw_vector& fft, std::ostream* out_stream, bool writeMagArg)
{
	std::size_t N = fft.size();
	int iN = static_cast<int>(N);
	double x, y;

	// The negative half-axis:
	for(std::size_t i=N/2; i<N; i++)
	{
		fft.getSample(x,y,i,writeMagArg);
		(*out_stream) << fft.frequency(i-iN) << ' ' << x << ' ' << y << '\n';
	}

	for(std::size_t i=0; i<N/2; i++)
	{
		fft.getSample(x,y,i,writeMagArg);
		(*out_stream) << fft.frequency(i) << ' ' << x << ' ' << y << '\n';
	}
}
///////////////////////////////////////////////////////////////////////////////
void writeStandard(fftw_vector& fft, std::ostream* out_stream,
		bool writeFirstHalfOnlyhalf, bool writeMagArg)
{
	std::size_t N = fft.size();
	double x, y;

	if(writeFirstHalfOnlyhalf) N/=2;

	for(std::size_t i=0; i<N; i++)
	{
		fft.getSample(x,y,i,writeMagArg);
		(*out_stream) << fft.frequency(i) << ' '<< x << ' ' << y << '\n';
	}
}
///////////////////////////////////////////////////////////////////////////////






