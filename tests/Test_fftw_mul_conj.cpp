#include "fftw_utils.h"
#include <iostream>

// testing the fftw_mul functions
int Test_fftw_mul_conj(int , char*[])
{
	int ret_ok=0, ret_fail=1;
	double epsilon = 1e-12;
	fftw_complex z, a, b, truth;
	a[0] = 1.0;
	a[1] = 1.0;

	b[0] = 1.0;
	b[1] = 1.0;

/////////////////////////////////////////////////
	fftw_mult_conj(z,a,b);
	
	std::cout << "a=(" << a[0] << ',' << a[1] << ")\n";
	std::cout << "b=(" << b[0] << ',' << b[1] << ")\n";

	std::cout << "z=a*conj(b)=(" << z[0] << ',' << z[1] << ")\n\n";

	truth[0]=2.0;
	truth[1]=0.0;
	if(std::abs(z[0]-truth[0])>epsilon || std::abs(z[1]-truth[1])>epsilon)
		return ret_fail;
	
/////////////////////////////////////////////////
	std::cout << "a=(" << a[0] << ',' << a[1] << ")\n";
	std::cout << "z=(" << z[0] << ',' << z[1] << ")\n";

	fftw_mult_conj(b,z,a);
	fftw_mult_conj(z,a);
	
	std::cout << "b=z*a= (" << b[0] << ',' << b[1] << ")\n";
	
	truth[0]=2.0;
	truth[1]=-2.0;
	if(std::abs(b[0]-truth[0])>epsilon || std::abs(b[1]-truth[1])>epsilon)
		return ret_fail;
	
	std::cout << "z*=a = (" << z[0] << ',' << z[1] << ")\n";

	if(std::abs(z[0]-truth[0])>epsilon || std::abs(z[1]-truth[1])>epsilon)
		return ret_fail;

/////////////////////////////////////////////////
// This test lets a complex unit vector rotate on the unit circle
// by repeatedly multiplying a delta angle (phi) to the original value of 1,0

	double phi=0.01;
	z[0] = 1.0;
	z[1] = 0.0;
	
	a[0] = cos(phi);  // a is the delta angle.
	a[1] = sin(phi);

	for(int i=1; i<100; ++i)
	{
		fftw_mult_conj(b,z,a); // multiplying z and a and store the result in b
		if(std::abs(b[0]-cos(-phi*i))>epsilon || std::abs(b[1]-sin(-phi*i))>epsilon)
			return ret_fail;
		
		fftw_mult_conj(z,a);   //multiplying z with a and assign the result to z.
		if(std::abs(z[0]-cos(-phi*i))>epsilon || std::abs(z[1]-sin(-phi*i))>epsilon)
			return ret_fail;
	}

	return ret_ok;
}





































