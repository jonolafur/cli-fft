/*
* fftOptions.cpp
*
*  Created on: Oct 29, 2011
*      Author: jon
*/

#include "fftOptions.h"



fftOptions::fftOptions(int argc, char* argv[]) :  programOptions()
{
   addfftOptions();
   loadOptionsFromCommandLine(argc, argv);
}


void fftOptions::addfftOptions()
{
   // declare all options:
   desc.add_options()
   ("input-file,i", po::value<string>(),
         "Name of input file. If nothing is specified, the standard input will be used.")
   ("output-file,o", po::value<string>(),
         "Name of output file. If nothing is specified, the standard output will be used.")
   ("delimiter,d", po::value<string>()->default_value( string(" ") ),
         "Delimiter string to be used for data fields in the output file. "
         "Please note that the a tabulator can be specified as \"tab\" or as \"\\t\".")
   ("input-delimiter,D", po::value<string>()->default_value( string(" ") ),
        "Delimiter assumed for the input data. Please note that if the data is separated by white space "
        "(tab or a varaible number of spaces), then leave this option empty. If you specify more than "
        "one character (e.g. \";,\") then either of those will be treated as a delimiter.")
   ("estimate-fftw,e","Only perform an estimate of the optimal fft algorithm. "
         "This tells fftw to generate plans using the FFTW_ESTIMATE flag.")
   ("ordered-x-axis,O","The natural output of the fft is to have the positive frequencies "
         "from sample index 1 to N/2 and the negative frequencies from sample index N/2 to N. "
         "With this option the frequency axis is ordered, i.e. the smallest frequency is at "
         "sample index 1 and the largest frequency at sample index N")
   ("positive-only,p", "If the input is real the spectrum is symmetric and only one half-axis carries "
         "information. This option suppresses the negative frequencies in the output if the input data is real.")
   ("magnitude-phase,M","Output format: Writes the magnitude and phase to the output, "
         "rather than real and imaginary components.")
   ("inverse,I", "Perform an inverse fft rather than a forward FFT.")
   ("auto-correlate,A", po::value<string>(), "Perform an auto-correlation of the samples. The operation basically does IFFT(FFT*conj(FFT)).\n"
         "To have more control over what\'s happening you must specify one of the following:\n"
         "\'c\' \tor \'cyclic\': Do a simple cyclic auto correlation.\n"
         "\'l\' \tor \'linear\': Do a simple linear auto correlation. This is done by zero padding the signal "
         "by a factor of two. This will down-weigh the higher lag values (implicit Bartlett window).\n"
         "\'b\' \tor \'no-bartlett\': Same as \'linear\', except the implicit Bartlett windowing is removed, i.e. the samples are "
         "normalized with N-|l|, where N is the number of samples and l is the lag.")
   ("complex,c","Perform transformations on complex data. See -y option below.")
   ("sample-rate,s", po::value<double>(), "Specify sample rate to generate a corresponding frequency axis.")
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
         "identical result is obtained")
   ("batches,B",po::value<size_t>()->default_value(1),"Number of batches to divide the sample set.")
   ("welch,w",po::value<double>()->default_value(0.0),
         "Welch\'s method. When averaging batches, this option specifies the overlap of the "
         "batches in percent.")
   ("average-mode,m",po::value<int>()->default_value(1),
         "Average the magnitude of the spectrum of each batch (see option \"batches\".):\n"
         "   \'1\': Power: the square of the magnitude of each batch is averaged.\n"
         "   \'2\': Magnitude: the magnitude of each batch is averaged.\n"
         "   \'3\': Linear: The results of each batch is simply averaged.\n")
   ("window,W",po::value<int>()->default_value(1),
         "Window function to be applied. Note that this window type is also applied to "
         "each of the batches, when averaging batches:\n"
         "   \'1\': Rectangular window.\n"
         "   \'2\': Bartlett window.\n"
         "   \'3\': Hann window.\n" );
}


///////////////////////////////////////////////////////////////////////////////
// This function returns a zero-based vector of indexes of the data columns of
// the data file provided by the user. The dimension is always 3 invalid
// entries are set to -1. For example if the user specifies -x 3, -y 5 then
// the function should return a vector with the entries [2,4,-1]. If the user
// specified complex data, the returned vector will be: [2,4,5]
std::vector<int> fftOptions::getZeroBasedColumnIndexes()
{
   std::vector<int> colIdx(3,-1);

   if(x_value_Idx()>0)
      colIdx[0] = x_value_Idx()-1;

   if(y_value_Idx()>0)
   {
      colIdx[1]= y_value_Idx()-1;

      if(isComplex())
         colIdx[2] = y_value_Idx();
   }

   return colIdx;
}


string fftOptions::delimiter() const
{
   string d = var_map["delimiter"].as<string>();
   string escaped_tab("\\t");

   if(d.find( escaped_tab ) != string::npos ||
      d.find( "tab" )       != string::npos )
   {
      return string("\t");
   }

   return d;
}


string fftOptions::inputFile()
{
   if(var_map.count("input-file") ==0)
      return string();

   return var_map["input-file"].as<string>();
}


string fftOptions::outputFile()
{
   if(var_map.count("output-file") ==0)
      return string();

   return var_map["output-file"].as<string>();
}


bool fftOptions::cyclicAcf() const
{
   if(var_map.count("auto-correlate")==0)
      return false;

   return ( var_map["auto-correlate"].as<std::string>() == "c" ||
            var_map["auto-correlate"].as<std::string>() == "cyclic" );
}


bool fftOptions::zeroPadSamples() const
{
   if(var_map.count("auto-correlate")==0)
      return false;

   // The linear auto correlation requires zero padding. There are two types of
   // linear ACF: with and without removal of the implicit Bartlett window.
   return ( var_map["auto-correlate"].as<string>() == "l" ||
         var_map["auto-correlate"].as<string>() == "linear" ||
         var_map["auto-correlate"].as<string>() == "b" ||
         var_map["auto-correlate"].as<string>() == "no-bartlett" );
}


bool fftOptions::removeBartlett() const
{
   if(var_map.count("auto-correlate")==0)
      return false;

   return ( var_map["auto-correlate"].as<string>() == "b" ||
            var_map["auto-correlate"].as<string>() == "no-bartlett" );
}


size_t fftOptions::getNumBatches() const
{
   return var_map["batches"].as<size_t>();
}


bool fftOptions::averageMagnitude() const
{
   return (var_map["average-mode"].as<int>() == 2);
}

bool fftOptions::averagePower() const
{
   return (var_map["average-mode"].as<int>() == 1);
}


bool fftOptions::averageLinear() const
{
   return (var_map["average-mode"].as<int>() == 3);
}


bool fftOptions::useBartlett() const
{
   return (var_map["window"].as<int>() == 2);
}


bool fftOptions::useRectangular() const
{
   return (var_map["window"].as<int>() == 1);
}


bool fftOptions::useHann() const
{
   return (var_map["window"].as<int>() == 3);
}

double fftOptions::welchOverlap(size_t batch_size) const
{
   double overlap = var_map["welch"].as<double>()/100.0;
   
   if(overlap >= 100.0 || overlap < 0.0)
      throw "Invalid overlap (welch option). The overlap is specified "
      "in percent. must be between 0 and 100.";

   size_t n = batch_size*overlap;

   if(n >= batch_size)
      throw "Specified overlap in the welch option results in 100% "
            "overlap due to rounding. PLease specify a smaller overlap.";

   return overlap;
}










