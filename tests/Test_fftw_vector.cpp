#include <cstdlib>
#include <sstream>
#include "fileIO.h"
#include "fftw_vector.h"
#include <boost/random.hpp>


///////////////////////////////////////////////////////////////////////////////
template<typename T>
bool compare(T x, T y, T tol)
{
	if(std::abs(x-y)>tol)
	{
		std::cout << "Expected value: " << y << " value obtained: " << x << std::endl;
		return false;
	}
	return true;
}
///////////////////////////////////////////////////////////////////////////////
bool compare_fftw_vec(fftw_vector& result, fftw_vector& truth, double tol )
{
	if(result.size() != truth.size() )
	{
		std::cout << "Unequal size of result and truth" << std::endl;
		return false;
	}
	
	for(std::size_t i=0; i<truth.size(); ++i)
	{
		if(std::abs(result.m_x[i][0] - truth.m_x[i][0])>tol ||
		   std::abs(result.m_x[i][1] - truth.m_x[i][1])>tol)
		{
			std::cout << "Deviation from truth exceeds allowed tolerance at index: " << i << '\n';
			std::cout << "Real part. Got: "  << result.m_x[i][0] << " expected to see: " << truth.m_x[i][0] << '\n';
			std::cout << "Imag. part. Got: " << result.m_x[i][1] << " expected to see: " << truth.m_x[i][1] << '\n';
			return false;
		}
	}
	return true;
}
bool checkSymmetryOfACF(fftw_vector& v,double tolerableError )
{
	if(v.size()%2!=0)
	{
		std::cout << "ACF has odd size." << std::endl;
		return false;
	}

	for(std::size_t i = 1; i<v.size()/2; i++)
	{
		if(std::abs(v[v.size()-i][0]-v[i][0]) > tolerableError || // even
           std::abs(v[v.size()-i][1]+v[i][1]) > tolerableError )  // odd
        {
			std::cout << "ACF not symmetric at index: " << i
					<< " Deviation was: (" << v[v.size()-i][0]-v[i][0] << " ,"
					<< v[v.size()-i][1]+v[i][1] << ")\n";
			return false;
        }
    }
	return true;
}
///////////////////////////////////////////////////////////////////////////////
bool test_fftw_inner_unequal_size()
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
		return true;
	}
	std::cout << "Error: Inner product did not throw exception for un-equal vector sizes!" << std::endl;
	return false;
}
///////////////////////////////////////////////////////////////////////////////
bool test_fftw_inner()
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
	{
		std::cout << "Failed to compare norm." << std::endl;
		return false;
	}

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
	{
		std::cout << "Failed multiplication with self." << std::endl;
		return false;
	}

	// Compare the sums of the real and imag part:
	x = 0.0;
	y = 0.0;
	for(std::size_t i =0; i<v.size(); ++i)
	{
		x += p[i][0];
		y += p[i][1];
	}

	if(!compare(x, 385.0, 1e-12) || !compare(y, 55.0, 1e-12) )
	{
		std::cout << "Failed multiplication." << std::endl;
		return false;
	}


	return true;
}
///////////////////////////////////////////////////////////////////////////////
bool test_acf_cyclic()
{
	fftw_vector v;
	double pi = 2.0*acos(0.0);
	double epsilon = 1.0e-7;
	int N = 512;
	double A = N*N;
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
			std::cout << "Failed acf_cyclic at index: " << i
					<< " Deviation was: ("
					<< A*cos(w*t)-v[i][0] << ", "<< A*sin(w*t)-v[i][1] << ")\n";
			return false;
		}
	}
	return true;
}
///////////////////////////////////////////////////////////////////////////////
bool test_acf_zero_pad(std::size_t N, double angularFrequency)
{
	data_columns cols;
	fftw_vector v;
	double pi = 2.0*acos(0.0);
	double tolerableError = 1.0e-7;
	double A = 2*N*N; // The expected amplitude: In N samples there is signal, but there are 2N samples
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
			std::cout << "Failed acf_zero_pad at index: " << i
					<< " Deviation was: ("
					<< A*cos(angularFrequency*t)-v[i][0] << ", "
					<< A*sin(angularFrequency*t)-v[i][1] << ")\n";
			return false;
		}
	}

	if(!checkSymmetryOfACF(v,tolerableError))
		return false;

	return true;
}

bool test_fftw_vector_normalize()
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
		return false;

	v.acf_normalized();

	std::cout << "test_fftw_vector_normalize: ACF(0): real: "
			<< v.real(0)<<  " imaginary: " << v.imag(0) << '\n';

	if(std::abs(v.abs(0)-1)>1e-8)
	{
		std::cout << "Failed norm ACF."<< std::endl;
		return false;
	}
	return true;

}

bool test_acf_GPS()
{
	std::ifstream in_code("ref-data/code_GPS_L1_01.txt");

	if(!in_code.good())
		throw "Test_fftw_vector:test_acf_GPS Failed to open reference code file.";

	std::string s;

	in_code >> s;

	std::cout << s << std::endl;

	throw "Unfinished test: Fails per default!!.";
   
	return false;
}

///////////////////////////////////////////////////////////////////////////////
// Main test driver:
int Test_fftw_vector(int , char*[])
{
	int ret_ok=0, ret_fail=1;

	if(!test_fftw_inner_unequal_size())
		return ret_fail;

	if(!test_fftw_inner())
		return ret_fail;

	if(!test_acf_cyclic())
		return ret_fail;

	if(!test_acf_zero_pad(512, 3.789))
		return ret_fail;

	if(!test_acf_zero_pad(511, 3.5))
		return ret_fail;

	if(!test_fftw_vector_normalize())
		return ret_fail;

	if(!test_acf_GPS())
		return ret_fail;

	return ret_ok;
}


































