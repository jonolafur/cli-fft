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
#include <boost/date_time/posix_time/posix_time.hpp>

// Fastest Fourier transform in the West:
#include <fftw3.h>  // Include Fast Fourier Transform Library

// Project headers:
#include "StreamHandler.h"
#include "LogStreamHandler.h"
#include "tty_cols.h"
#include "fftw_vector.h"
#include "fftOptions.h"
#include "fileIO.h"
///////////////////////////////////////////////////////////////////////////////
// short hand for program option name space...
namespace po = boost::program_options;
namespace bpt = boost::posix_time;

///////////////////////////////////////////////////////////////////////////////
void process(fftOptions& opt);
void readInput(fftOptions& opt, fftw_vector& fft_vec, StreamHandler& stream_handler);
void processInput(fftOptions& opt, fftw_vector& fft_vec);
void writeOutput(fftOptions& opt, fftw_vector& fft_vec);
double checkSampleDelta(const std::vector<double>& t);
void writeOrdered(fftw_vector& fft, std::ostream* out_stream,
                  const fftOptions& opt);
void writeStandard(fftw_vector& fft, std::ostream* out_stream,
                   const fftOptions& opt);
void writeSample(int idx, int offset, fftw_vector& fft, std::ostream* out_stream, const fftOptions& opt);
void logHistory( int a, char* av[] );

#endif //_RX_MAIN_H



