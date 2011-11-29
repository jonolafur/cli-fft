///////////////////////////////////////////////////////////////////////////////
// Description:
//   Defines the entry point for the fft console application.
//   This is a very simple application that performs fft and ifft on data sets
//   in ASCII files and outputs on std out or into file
///////////////////////////////////////////////////////////////////////////////
// SVN Header (The following information will be automatically updated by SVN)
// $Author: $
// $Date:  $
// $Revision: $
// $Id:$
///////////////////////////////////////////////////////////////////////////////
// Include guards:
#ifndef _FFT_MAIN_H
#define _FFT_MAIN_H
//////////////     Include section     ////////////////////////////////////////
// STL headers:
#include <string>
#include <iostream>
#include <iomanip>

// Boost headers:
#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>

// Fastest Fourier transform in the West:
#include <fftw3.h>  // Include Fast Fourier Transform Library

// Project headers:
#include "tty_cols.h"
#include "fftw_vector.h"
#include "fftOptions.h"
#include "fileIO.h"
///////////////////////////////////////////////////////////////////////////////
// short hand for program option name space...
namespace po = boost::program_options;

///////////////////////////////////////////////////////////////////////////////
void readInput(fftOptions& opt, fftw_vector& fft_vec);
void processInput(fftOptions& opt, fftw_vector& fft_vec);
void writeOutput(fftOptions& opt, fftw_vector& fft_vec);
void writeOutput(fftOptions& opt, fftw_vector& fft_vec);
double checkSampleTime(const std::vector<double>& t);
void writeOrdered(fftw_vector& fft, std::ostream* out_stream, bool writeMagArg);
void writeStandard(fftw_vector& fft, std::ostream* out_stream,
		bool writeFirstHalfOnlyhalf, bool writeMagArg);
std::streambuf* redirect_clog(std::string log_file_base_name);


#endif //_RX_MAIN_H



