#include <cstdlib>
#include <sstream>
#include "fileIO.h"
#include "fftw_vector.h"
#include "Test_helpers.h"
#include <boost/random.hpp>
#include <boost/lexical_cast.hpp>

void checkSymmetryOfACF(fftw_vector& v,double tolerableError )
{
	if(v.size()%2!=0)
      throw "ACF has odd size.";

	for(std::size_t i = 1; i<v.size()/2; i++)
	{
      std::stringstream ss;
		if(std::abs(v[v.size()-i][0]-v[i][0]) > tolerableError || // even
           std::abs(v[v.size()-i][1]+v[i][1]) > tolerableError )  // odd
        {
         ss << "ACF not symmetric at index: " << i
					<< " Deviation was: (" << v[v.size()-i][0]-v[i][0] << " ,"
					<< v[v.size()-i][1]+v[i][1] << ")\n";
         throw ss.str();
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
void test_fftw_inner_unequal_size()
{
	fftw_vector v;
	fftw_vector p;

	v.re_alloc(5, false);
	p.re_alloc(4, false);

	try
	{
		v.mult_conj(p);
	}
	catch(const char* s)
	{
		std::cout << "Test is supposed to throw an exception: \n"
				<< s << std::endl;
      return;
	}
   throw "Error: Inner product did not throw exception for un-equal vector sizes!";
}
///////////////////////////////////////////////////////////////////////////////
void test_fftw_inner()
{
	fftw_vector v;
	fftw_vector p;

	v.re_alloc(5, false);
	p.re_alloc(5, false);

	// Initializing:
	double x = 1.0;
	for(std::size_t i =0; i<v.size(); ++i, x += 1.0)
	{
		v[i][0] = x;
		v[i][1] = x*2.0;

		p[i][0] = x;
		p[i][1] = x*3.0;
	}

	//////////
	// Test norm:
	double n = v.norm();
	if(!compare(n, 16.5831239518, 1e-8))
      throw "Failed to compare norm.";

	// Multiply:
	p.mult_conj(v);
	// Multiplying with self:
	v.mult_conj(v);

	x = 0.0;
	double y = 0.0;
	for(std::size_t i =0; i<v.size(); ++i)
	{
		x += v[i][0];
		y += v[i][1];
	}

	if(!compare(sqrt(x), 16.5831239518, 1e-8) || !compare(sqrt(y), 0.0, 1e-8) )
      throw "Failed multiplication with self.";

	// Compare the sums of the real and imag part:
	x = 0.0;
	y = 0.0;
	for(std::size_t i =0; i<v.size(); ++i)
	{
		x += p[i][0];
		y += p[i][1];
	}

	if(!compare(x, 385.0, 1e-12) || !compare(y, 55.0, 1e-12) )
      throw "Failed multiplication.";

   return;
}
///////////////////////////////////////////////////////////////////////////////
void test_acf_cyclic()
{
	fftw_vector v;
	double pi = 2.0*acos(0.0);
	double epsilon = 1.0e-7;
	int N = 512;
   double A = N;
	double dt = 2.0*pi/double(N);
	double t = 0.0;
	double w = 3.0;

	v.re_alloc(N, false);

	for(std::size_t i =0; i<v.size(); ++i, t += dt)
	{
		v[i][0] = cos(w*t);
		v[i][1] = sin(w*t);
	}

	v.acf();

	// v.write("dbg_cyclic.txt");

	// Check result:
	t=0.0;
	for( std::size_t i=0; i<v.size(); ++i, t += dt)
	{
		if(std::abs(A*cos(w*t)-v[i][0]) >epsilon ||
		   std::abs(A*sin(w*t)-v[i][1]) >epsilon )
		{
         std::stringstream ss;
         ss << "Failed acf_cyclic at index: " << i << " Deviation was: ("
					<< A*cos(w*t)-v[i][0] << ", "<< A*sin(w*t)-v[i][1] << ")\n";
         throw ss.str();
		}
	}
   return;
}
///////////////////////////////////////////////////////////////////////////////
void test_acf_zero_pad(std::size_t N, double angularFrequency)
{
	data_columns cols;
	fftw_vector v;
	double pi = 2.0*acos(0.0);
	double tolerableError = 1.0e-7;
   double A = N; // The expected amplitude: There are 2N samples, but only N samples with signal.
                 // However, fftw_vector normalizes with number of samples.
	double dt = 2.0*pi/double(N);
	double t = 0.0;

	for(std::size_t i =0; i<N; ++i, t += dt)
	{
		cols.m_data[1].push_back(cos(angularFrequency*t));
		cols.m_data[2].push_back(sin(angularFrequency*t));
	}

	cols.zeroPadSamplesByFactorTwo();

	v.set_samples(cols.realAxis(), cols.imagAxis(),false);

	v.acf(true);

//	v.write("dbg.txt");

	// Check result:
	t=0.0;
	for( std::size_t i=0; i<N; ++i, t += dt)
	{
		if(std::abs(A*cos(angularFrequency*t)-v[i][0]) >tolerableError ||
		   std::abs(A*sin(angularFrequency*t)-v[i][1]) >tolerableError )
		{
         std::stringstream ss;
         ss << "Failed acf_zero_pad at index: " << i << " Deviation was: ("
					<< A*cos(angularFrequency*t)-v[i][0] << ", "
					<< A*sin(angularFrequency*t)-v[i][1] << ")\n";
         throw ss.str();
		}
	}

   checkSymmetryOfACF(v,tolerableError);

   return;
}

void test_fftw_vector_normalize()
{
	boost::mt19937 rng( 1234 );  // on gcc the cast will use the LSBs
	boost::normal_distribution<> gauss_dist;   // Gauß-ian distribution with mean 0 and variance 1
	boost::variate_generator<boost::mt19937&, boost::normal_distribution<> >
           gauss(rng, gauss_dist);             // glues randomness with mapping

	int N = 1000;
	fftw_vector v;
	v.re_alloc(N,true);

	// Fill up with some garbage:
	fftw_complex z;

	for(int i=0; i<N; i++)
	{
		z[0] = gauss();
		z[1] = gauss();

		v.push_back(z);
	}

	double norm_before = v.norm();

	v.fft();
	v.ifft();

	v.normalizeSquare();
	double norm_after = v.norm();

	std::cout << "test_fftw_vector_normalize: Norm of random garbage before: " << norm_before
			<< " norm after: " << norm_after << '\n';

	if(std::abs(norm_after-norm_before) >1e-6)
      throw "Comparision of norms failed in test_fftw_vector_normalize";

	v.acf_normalized();

	std::cout << "test_fftw_vector_normalize: ACF(0): real: "
			<< v.real(0)<<  " imaginary: " << v.imag(0) << '\n';

	if(std::abs(v.abs(0)-1)>1e-8)
      throw "Failed norm ACF.";

   return;
}

void test_acf_GPS(bool zeroPad, bool removeBartlett)
{
   fftw_vector v;
   std::string s;
   data_columns cols;
   std::ifstream in_code("ref-data/code_GPS_L1_01.txt");
   std::size_t expectedSize = 1023;

	if(!in_code.good())
		throw "Test_fftw_vector:test_acf_GPS Failed to open reference code file.";

	in_code >> s;

   for(auto i=0UL; i<s.length();i++)
   {
      cols.m_data[1].push_back(2.0*(0.5-boost::lexical_cast<double>(s[i])));
      cols.m_data[2].push_back(0.0);
   }

   if(zeroPad)
   {
      cols.zeroPadSamplesByFactorTwo();
      expectedSize *=2;
   }

   v.set_samples(cols.realAxis(), cols.imagAxis(),true);

   if(v.size()!=expectedSize)
      throw "Wrong size in test_acf_GPS. Should be 1023 or 2046";

   v.acf(removeBartlett && zeroPad);

   v.write("acf_dbg.txt", false);

   // The ACF of the GPS codes has values 1023 at zero:
   if(!compare(v[0][0],1023.0, 1e-12))
      throw "ACF at zero was not 1023";

   if(zeroPad)
      return;

   // The side lobes of the GPS ACF take on the values -1, 63 or -65
   for(auto i=1ul; i<v.size();i++)
   {
      if(!compare(v[i][1],0.0, 1e-12))
         throw "Imaginary part of ACF not zero";

      if(!compare(v[i][0],-1.0, 1e-12) &&
         !compare(v[i][0],63.0, 1e-12) &&
         !compare(v[i][0],-65.0, 1e-12))
         throw "Unexpected value of real part of ACF";
   }

   return;
}

void acf_random(bool zeroPad, bool removeBartlett)
{
   boost::mt19937 rng( 123456 );  // on gcc the cast will use the LSBs
   boost::normal_distribution<> gauss_dist;   // Gauß-ian distribution with mean 0 and variance 1
   boost::variate_generator<boost::mt19937&, boost::normal_distribution<> >
           gauss(rng, gauss_dist);             // glues randomness with mapping

   int N = 1000;
   fftw_vector v;
   v.re_alloc(N,true);

   for(int i=0; i<N; i++)
      v.push_back(gauss(), gauss());

   v.acf(removeBartlett && zeroPad);

   v.write("acf_random.txt", false);

}

///////////////////////////////////////////////////////////////////////////////
// Main test driver:
int Test_fftw_vector(int , char*[])
{
   auto ret_ok=0, ret_error=1;

   try
   {
      test_fftw_inner_unequal_size();

      test_fftw_inner();

      test_acf_cyclic();

      test_acf_zero_pad(512, 3.789);

      test_acf_zero_pad(511, 3.5);

      test_fftw_vector_normalize();

      test_acf_GPS(false, false);

      test_acf_GPS(true, false);

      test_acf_GPS(true, true);

      acf_random(true, true);
   }
   catch(std::exception& e)
   {
      std::cout << "Error: " << e.what() << std::endl;
      return ret_error;
   }
   catch(std::string& s)
   {
      std::cout << "Error: " << s << std::endl;
      return ret_error;
   }
   catch(const char* s)
   {
      std::cout << "Error: " << s << std::endl;
      return ret_error;
   }
   catch(...)
   {
      std::cout << "Exception of unknown type!\n";
      return ret_error;
   }

   return ret_ok;
}


































