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
	std::string log_file_name(".fft.log");
	std::ofstream log_file;
	std::streambuf* backup=0L;
	
	try
	{
		log_file_name = makeLogPath(log_file_name);
		backup = redirect_clog(log_file_name, log_file );

		logHistory(argc, argv);

		fftOptions opt(argc, argv);

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
		
		process(opt);

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

	if(ret_error)
		std::cout << "# Errors were encountered, please see log file: " << log_file_name << std::endl;

	return ret_val;
}
///////////////////////////////////////////////////////////////////////////////
void process(fftOptions& opt)
{
	fftw_vector fft_vec;
	StreamHandler stream_handler(opt.inputFile());

	readInput(opt, fft_vec, stream_handler);
	processInput(opt,fft_vec);
	writeOutput(opt,fft_vec);
}
///////////////////////////////////////////////////////////////////////////////
void readInput(fftOptions& opt, fftw_vector& fft_vec, StreamHandler& stream_handler)
{
	data_columns fft_data;

	std::vector<int> colIdx = opt.getZeroBasedColumnIndexes();

	getColsFromFile(stream_handler, fft_data, colIdx, opt.getDelimiterAsString() );

	if(opt.zeroPadSamples())
		fft_data.zeroPadSamplesByFactorTwo();

	if(opt.isComplex())
		fft_vec.set_samples(fft_data.realAxis(), fft_data.imagAxis(), opt.estimate_fftw());
	else
		fft_vec.set_samples(fft_data.realAxis(), opt.estimate_fftw());

	if(opt.xAxisSpecified())
	{
		double dx =  checkSampleDelta(fft_data.x_axis());
		if(opt.inverseFFT())
			fft_vec.setSampleFrequency(dx);
		else
			fft_vec.setSampleTime(dx);
	}
}
///////////////////////////////////////////////////////////////////////////////
void processInput(fftOptions& opt, fftw_vector& fft_vec)
{
	if(opt.acf())
	{
		fft_vec.acf(opt.removeBartlett());

		if( opt.normalize() )
			fft_vec.normalizeSquare();

		return;
	}

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
	if(!opt.outputFile().empty())
	{
		out_file.open(opt.outputFile().c_str());
		out_stream = &out_file;
	}
	else
		out_stream = &std::cout;

	(*out_stream).precision(12);

	if(opt.orderSamples())
		writeOrdered(fft_vec, out_stream, opt);

	writeStandard(fft_vec, out_stream, opt );

}
///////////////////////////////////////////////////////////////////////////////
double checkSampleDelta(const std::vector<double>& t)
{
	std::size_t N = t.size();
	if(N<2) throw "An x-axis was specified on the command line (option -x), but in "
			"that column less than two samples were found.";

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
		writeSample(i, 0, fft, out_stream, opt);
}
///////////////////////////////////////////////////////////////////////////////
void writeSample(int idx, int offset, fftw_vector& fft, std::ostream* out_stream, const fftOptions& opt)
{
	double a,x,y;

	// If an IFFT was performed, the assumption is that the horizontal axis of
	// the _output_ (i.e. _after_ the transformation) is a time coordinate
	if(opt.inverseFFT() || opt.acf())
		a = fft.time(idx-offset);
	else // Otherwise it is assumed to be a frequency coordinate.
		a = fft.frequency(idx-offset);

	fft.getSample(x, y, idx, opt.writeMagnitudeAndPhase() );

	(*out_stream) << a << ' ' << x << ' ' << y << '\n';
}
///////////////////////////////////////////////////////////////////////////////
std::streambuf* redirect_clog(std::string log_path,
                              std::ofstream& log_file)
{
	std::streambuf* clog_buffer = std::clog.rdbuf();

	log_file.open(log_path.c_str());

	if(log_file)
		std::clog.rdbuf( log_file.rdbuf() );
	else
	{
		std::string s = "Failed to open file: " + log_path + "for writing.\n";
		throw s;
	}

	return clog_buffer;
}
///////////////////////////////////////////////////////////////////////////////
std::string makeLogPath(std::string log_file_base_name)
{
	bool readHomePathOk=false;
	std::string log_path;

	char* homePath = getenv("HOME");

	if(homePath)
	{
		readHomePathOk=true;
		log_path = std::string( homePath ) + "/" + log_file_base_name;
	}
	else
		log_path = log_file_base_name;

	if(!readHomePathOk)
		std::cout << "# Unable to retrieve home path. Writing log to current directory.\n";

	return log_path;
}
///////////////////////////////////////////////////////////////////////////////
void logHistory( int a, char* av[] )
{
	bpt::ptime t(bpt::second_clock::local_time());

	std::clog << "# " << t << std::endl;
	// Print out the options, skip the program name...
	for(int i=1; i<a;++i)
		std::clog << av[i] << ' ';
	std::clog << std::endl;
}




