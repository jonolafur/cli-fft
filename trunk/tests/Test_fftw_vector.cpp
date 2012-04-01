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
		v.mult_conjugate(p);
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
	p.mult_conjugate(v);
	// Multiplying with self:
	v.mult_conjugate(v);


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
// testing the fftw_mul functions
bool test_fftw_tukey()
{
	int ret_ok=0;
	double epsilon=1e-12;
	fftw_vector v, truth;

/////////////
	// The vector has not been allocated yet. Here the function
	// shall just return quietly
	std::cout << "Applying Tukey window to empty fftw_vector. Should return quietly.\n";
	v.applyTukey(0.0);
	
/////////////
	// Here only four values are allocated. All values will be
	// initialized with (1,1).
	std::size_t N=4;
	v.re_alloc(N, true);
	truth.re_alloc(N, true);

	std::cout << "Applying Tukey to 4 values all (1.0,1.0) and alpha 2/3\n";
	for(std::size_t i=0; i<N; i++)
	{
		v.m_x[i][0] = 1.0;
		v.m_x[i][1] = 1.0;

		truth.m_x[i][0] = 1.0;
		truth.m_x[i][1] = 1.0;
	}
	
	truth.m_x[0][0] = truth.m_x[0][1] = truth.m_x[N-1][0] = truth.m_x[N-1][1] = 0.0;

	v.applyTukey(2.0/3.0);
	
	if(!compare_fftw_vec(v, truth, epsilon))
		return false;
	
/////////////
	std::cout << "Applying Tukey to 4 values all (1.0,1.0) and alpha 1/2\n";
	for(std::size_t i=0; i<N; i++)
	{
		v.m_x[i][0] = 1.0;
		v.m_x[i][1] = 1.0;
	}

	v.applyTukey(0.5);
	
	if(!compare_fftw_vec(v, truth, epsilon))
		return false;

////////////
	N = 200;
	v.re_alloc(N, true);
	std::cout << "Applying Tukey to 200 values all (1.0,1.0) and alpha 1/2" << std::endl;
	for(std::size_t i=0; i<N; i++)
	{
		v.m_x[i][0] = 1.0;
		v.m_x[i][1] = 1.0;
	}

	v.applyTukey(0.5);

	v.write("test_200.dat");
	std::stringstream command;
	command << "../../ascii_compare -t test_200.dat -r ref-data/Test_fftw_Tukey_200.txt"
	<< " -C 0 -d " << epsilon << " -D " << epsilon
	<< " -C 1 -d " << epsilon << " -D " << epsilon
	<< " -C 2 -d " << epsilon << " -D " << epsilon;
	int k = system( command.str().c_str() );

	if(k!=ret_ok)
	{
		std::cout << "File compare with test_200.dat failed with return value : "<< k <<'\n';
		return false;
	}

////////////
	N = 201;
	v.re_alloc(N, true);
	std::cout << "Applying Tukey to 201 values all (1.0,1.0) and alpha 1/2. "
	<< "Want to make sure an odd number of samples are also processed correctly." << std::endl;

	for(std::size_t i=0; i<N; i++)
	{
		v.m_x[i][0] = 1.0;
		v.m_x[i][1] = 1.0;
	}

	v.applyTukey(0.5);

	v.write("test_201.dat");
	command.str("");
	command << "../../ascii_compare -t test_201.dat -r ref-data/Test_fftw_Tukey_201.txt"
	<< " -C 0 -d " << epsilon << " -D " << epsilon
	<< " -C 1 -d " << epsilon << " -D " << epsilon
	<< " -C 2 -d " << epsilon << " -D " << epsilon;
	k = system( command.str().c_str() );

	if(k!=ret_ok)
	{
		std::cout << "File compare with test_201.dat failed with return value : "<< k <<'\n';
		return false;
	}

///////////////
	std::cout << "Applying Tukey to 201 values all (1.0,1.0) and alpha 0. "
	<< "Want to make sure an odd number of samples are also processed correctly." << std::endl;

	for(std::size_t i=0; i<N; i++)
	{
		v.m_x[i][0] = 1.0;
		v.m_x[i][1] = 1.0;
	}

	v.applyTukey(0.0);

	v.write("test_201_alpha_0.dat");
	command.str("");
	command << "../../ascii_compare -t test_201_alpha_0.dat -r ref-data/Test_fftw_Tukey_201_alpha_0.txt"
	<< " -C 0 -d " << epsilon << " -D " << epsilon
	<< " -C 1 -d " << epsilon << " -D " << epsilon
	<< " -C 2 -d " << epsilon << " -D " << epsilon;
	k = system( command.str().c_str() );

	if(k!=ret_ok)
	{
		std::cout << "File compare with test_201_alpha_0.dat failed with return value : "<< k <<'\n';
		return false;
	}

return true;
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

	if(!test_fftw_tukey())
		return ret_fail;

	return ret_ok;
}


































