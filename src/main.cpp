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
	std::ofstream log_file;
	std::streambuf* backup=0L;
	
	try
	{
		backup = redirect_clog(std::string(".fft.log"), log_file );

		fftOptions opt;
		opt.loadOptionsFromCommandLine(argc, argv);

		if(opt.writeVersionToConsole(programName))
		{
			std::clog.rdbuf(backup);
			return ret_ok;
		}

		if(opt.writeHelpToConsole())
		{
			std::clog.rdbuf(backup);
			return ret_ok;
		}

		fftw_vector fft_vec;

		readInput(opt, fft_vec);
		processInput(opt,fft_vec);
		writeOutput(opt,fft_vec);

	}
	catch(std::exception& e)
	{
		std::clog << "Error: " << e.what() << std::endl;
		ret_val = ret_error;
	}
	catch(std::string& s)
	{
		std::clog << "Error: " << s << std::endl;
		ret_val = ret_error;
	}
	catch(const char* s)
	{
		std::clog << "Error: " << s << std::endl;
		ret_val = ret_error;
	}
	catch(...)
	{
		std::clog << "Exception of unknown type!\n";
		ret_val = ret_error;
	}

	std::clog.rdbuf(backup);

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

	if( opt.normalize() )
		fft_vec.normalize();
}
///////////////////////////////////////////////////////////////////////////////
void writeOutput(fftOptions& opt, fftw_vector& fft_vec)
{
	std::ostream* out_stream;
	std::ofstream out_file;

	// Re-direct to standard out if no filename is given:
	if(!opt.outFileName.empty())
	{
		out_file.open(opt.outFileName.c_str());
		out_stream = &out_file;
	}
	else
		out_stream = &std::cout;

	if(opt.orderSamples())
		writeOrdered(fft_vec,out_stream, opt);

	writeStandard(fft_vec, out_stream, opt );

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
void writeOrdered(fftw_vector& fft, std::ostream* out_stream, const fftOptions& opt)
{
	int N = static_cast<int>(fft.size());

	// The negative half-axis:
	for(int i=N/2; i<N; i++)
		writeSample(i, N, fft,  out_stream, opt);

	for(int i=0; i<N/2; i++)
		writeSample(i, 0, fft,  out_stream, opt);
}
///////////////////////////////////////////////////////////////////////////////
void writeStandard(fftw_vector& fft, std::ostream* out_stream, const fftOptions& opt)
{
	int N = static_cast<int>(fft.size());

	if( opt.positiveAxisOnly() ) N/=2;

	for(int i=0; i<N; i++)
		writeSample(i, 0, fft,  out_stream, opt);
}
///////////////////////////////////////////////////////////////////////////////
void writeSample(int idx, int offset, fftw_vector& fft, std::ostream* out_stream, const fftOptions& opt)
{
	double a,x,y;

	if(opt.inverseFFT())
		a = fft.frequency(idx-offset);
	else
		a = fft.time(idx-offset);

	fft.getSample(x,y,idx,opt.writeMagnitudeAndPhase() );
	(*out_stream) << a << ' ' << x << ' ' << y << '\n';
}
///////////////////////////////////////////////////////////////////////////////
std::streambuf* redirect_clog(std::string log_file_base_name,
                              std::ofstream& log_file)
{
	std::streambuf* clog_buffer = std::clog.rdbuf();

	std::string log_path( getenv("HOME") );
	log_path += "/" + log_file_base_name;

	log_file.open(log_path.c_str());

	if(log_file)
		std::clog.rdbuf( log_file.rdbuf() );
	else
	{
		std::string s = "Failed to open file: " + log_file_base_name + "for writing.";
		throw s;
	}
	return clog_buffer;
}





