///////////////////////////////////////////////////////////////////////////////
//                              IfEN GmbH                                    //
///////////////////////////////////////////////////////////////////////////////
// Description:
//   A tiny file compare utility to be used with ctest. It is intended for
//   comapring ASCII files containing columns of numbers
// Dependencies (others than those explicitly visible in the #include clause):
//    None.
// Project: NA
///////////////////////////////////////////////////////////////////////////////
// SVN Header (The following information will be automatically updated by SVN)
// $Author: jon $
// $Date: 2010-02-07 11:43:45 +0100 (Sun, 07 Feb 2010) $
// $Revision: 343 $
// $State: Exp $
// $Id: main.cpp 343 2010-02-07 10:43:45Z jon $
///////////////////////////////////////////////////////////////////////////////
//Includes: ///////////////////////////////////////////////////////////////////
#include "ascii_compare.h"
#include <boost/iterator/iterator_concepts.hpp>
///////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
{
	const int ret_error = 1;
	const int ret_ok = 0;
	int ret_val = ret_ok;
	config cfg;
	po::variables_map vm;
	po::options_description desc("Available options");

	try
	{
		// declare all options:
		desc.add_options()
			("help,?", "Produce help message")
			("version,v", "Show the version of the program")
			("test-file,t", po::value<std::string>(&cfg.test_file_name), "The ascii file under test.")
			("reference-file,r", po::value<std::string>(&cfg.ref_file_name), "The ascii reference file.")
			("columns,C", po::value<std::vector<int> >(&cfg.columns),
			    "Picks out the columns to compare. The options \'C\', \'d\' and \'D\' must be "
			    "specified an equal number of times. The column index is zero-based. "
				 "Note that in this version it is assumed that the test file and the reference "
				 "file have the same structure. I.e. the column to compare has the same index for both files.")
			("deviation,d", po::value<std::vector<double> >(&cfg.tol_deviation),
			    "The tolerable mean deviation between each entry. If the mean deviation "
			    "exceeds this threshold, the test will fail.")
			("max-deviation,D", po::value< std::vector<double> >(&cfg.tol_max_deviation),
			    "The maximum tolerable deviation per column. If the deviation between any two entries "
			    "exceeds this threshold, the test will fail." )
			("comment,c", po::value<char>(&cfg.comment)->default_value('#'),
			    "Lines starting with this character will be ignored")
			("separator,s", po::value<char>(&cfg.delimiter)->default_value(' '),
			    "Token separating the columns. White space is the default." )
			("num-expected-col,N", po::value<int>(&cfg.num_expected_lines)->default_value(0),
			    "Number of expected successfully compared lines. If set to zero, this is ignored, "
				 "otherwise the test fails if the number of lines compared is not equal to N" );

		// Combine all options before parsing:
		po::options_description all_options;
      all_options.add(desc);

		// store and notify:
		po::store(po::parse_command_line(argc, argv, all_options), vm);
		po::notify(vm);

		if( vm.count("help") || argc==1 )	// if help was requested, print usage...
		{
			std::cout << desc << std::endl;
			return ret_ok;
		}

		if(vm.count("version"))
		{
			std::cout << "This is ascii_compare, version: " << ifen::version_string << std::endl
						<< "Built on: " << ifen::build_date_string << std::endl;
			return ret_ok;
		}

		if(vm.count("test-file")==0 || vm.count("reference-file")==0)
		{
			std::cout << "Please provide a test file and reference file name. Both are mandatory." << std::endl;
			std::cout << desc << std::endl;
			return ret_error;
		}
		if(vm.count("columns")!=vm.count("deviation") || vm.count("columns")!=vm.count("max-deviation") )
		{
			std::cout << "The options \'C\', \'d\' and \'D\' must be "
			    "specified an equal number of times ." << std::endl;
			std::cout << desc << std::endl;
			return ret_error;
		}
		if(vm.count("columns")==0)
		{
			std::cout << "Please specify which columns to compare using the option -C."
							 << std::endl << desc << std::endl;
			return ret_error;
		}

		compareFiles(cfg);   // Read the input file.
		compareResults(cfg); // here any exception will be thrown if the test failed

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
	catch(...)
	{
		std::cerr << "Exception of unknown type!\n";
		ret_val = ret_error;
	}

	return ret_val;
}
///////////////////////////////////////////////////////////////////////////////
void compareFiles(config& cfg)
{
	std::cout << "Comparing test file: " << cfg.test_file_name << std::endl;
	std::cout << "with the reference file: " << cfg.ref_file_name << std::endl;
	std::cout << "Delimiter is: ";
	if(cfg.delimiter ==' ')
		std::cout << "white space." << std::endl;
	else
		std::cout << '\'' << cfg.delimiter << '\'' << std::endl;

	// Read input file:
	std::string ref_line, test_line;
	cfg.num_compared_lines = 0;
	std::ifstream reference(cfg.ref_file_name.c_str());
	std::ifstream under_test(cfg.test_file_name.c_str());
	std::vector<double> ref_val, test_val;

	// Allocate vectors statistics for each column:
	cfg.deviation.resize(cfg.columns.size());
	cfg.max_deviation.resize(cfg.columns.size());
	cfg.max_dev_index.resize(cfg.columns.size(), 0);
	for(std::size_t i=0; i<cfg.deviation.size();++i)
		cfg.deviation[i]=cfg.max_deviation[i]=0.0;

	checkStream(reference);
	checkStream(under_test);

	do
 	{
 		ref_line = getNextLine(reference, cfg.comment);
 		test_line = getNextLine(under_test, cfg.comment);

		if( !reference.fail() && !under_test.fail() )
			++cfg.num_compared_lines;

 		ref_val =  get_fields<double>(ref_line, cfg.columns, cfg.delimiter);
 		test_val = get_fields<double>(test_line, cfg.columns, cfg.delimiter);

		for(std::size_t i=0; i< ref_val.size(); ++i)
		{
			double dev = std::abs( ref_val[i]-test_val[i] );
			cfg.deviation[i] += dev;
			if(dev>cfg.max_deviation[i])
			{
				cfg.max_deviation[i]=dev;
				cfg.max_dev_index[i]=cfg.num_compared_lines;
			}
		}
	} while( !reference.fail() && !under_test.fail() );

	for(std::size_t i=0; i<cfg.deviation.size();++i)
		cfg.deviation[i] /= double(cfg.num_compared_lines);
}
///////////////////////////////////////////////////////////////////////////////
void compareResults(config& cfg)
{
	bool ok=true;
	std::stringstream sstr;
	std::string s;
	std::size_t N = cfg.tol_deviation.size();
	if(N != cfg.columns.size() || N != cfg.max_deviation.size() ||
		N != cfg.tol_max_deviation.size() || N != cfg.deviation.size())
	{
		s="Number of data vectors inconsistent.";
		throw s;
	}

	for(std::size_t i=0; i<N; i++)
	{
		std::cout << "Column: " << cfg.columns[i] << " Mean deviation: " <<  cfg.deviation[i] << " Allowed: " << cfg.tol_deviation[i]
			<< " Max deviation: " <<  cfg.max_deviation[i] << " Allowed: " << cfg.tol_max_deviation[i] << std::endl;
		if(cfg.tol_deviation[i]<cfg.deviation[i])
		{
			sstr << "Tolerable mean deviation exceeded in column: " << cfg.columns[i] << '\n';
			ok = false;
		}

		if(cfg.tol_max_deviation[i]<cfg.max_deviation[i])
		{
			sstr << "Tolerable maximum deviation exceeded in column: " << cfg.columns[i]
			     << " Maximum deviation at index: "<< cfg.max_dev_index[i];
			ok = false;
		}

		if(!ok)
			throw sstr.str();

	}
	// This check is ignored if the user says he expects zero lines. Its default
	if(cfg.num_expected_lines!=0 && cfg.num_compared_lines!=cfg.num_expected_lines)
	{
		sstr << "Number of lines compared and number of lines expected not equal. " << std::endl
		<< "Number of expected lines: " << cfg.num_expected_lines << std::endl
		<< "Number of compared lines: " << cfg.num_compared_lines << std::endl;
		throw sstr.str();
	}
}
///////////////////////////////////////////////////////////////////////////////
std::string getNextLine(std::ifstream& in, char comment)
{
	std::string s;
	std::getline(in, s);	// Get the next line...

	while(in.good())	// ignore comments.
	{
		if(!s.empty())
			if(s[0]!=comment) break;
		std::getline(in, s);
	}
	return s;
}
///////////////////////////////////////////////////////////////////////////////
void checkStream(std::ifstream& in)
{
 	if(!in.good())
 	{
 		std::string s("Failed to open input file.");
 		std::logic_error e(s);
 		throw e;
 	}
}























