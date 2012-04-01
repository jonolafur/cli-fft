#include <cstdlib>
#include <sstream>
#include "fftw_vector.h"


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
bool test_acf()
{
	fftw_vector v;
	double pi = 2.0*acos(0.0);

	v.re_alloc(100, false);

	// Initializing:
	double dt = 2.0*pi/double(v.size());
	double t = 0.0;
	double w = 2.2;

	for(std::size_t i =0; i<v.size(); ++i, t += dt)
	{
		v[i][0] = cos(w*t);
		v[i][1] = sin(w*t);
	}

	v.fft();
	v.mult_conj(v);
	v.ifft();

	v.write("tmp.txt", false);

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

	if(!test_acf())
		return ret_fail;

	return ret_ok;
}


































