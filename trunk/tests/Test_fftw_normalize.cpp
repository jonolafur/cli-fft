#include "fftw_utils.h"
#include <iostream>

double epsilon=1e-12;

bool test_normalize( fftw_complex& a )
{

	double r=sqrt(a[0]*a[0]+a[1]*a[1]);
	double phi = atan2(a[1],a[0]);

	std::cout << "Length before: " << r << std::endl;
	std::cout << "Argument before: " << phi << std::endl;
	
	fftw_normalize(a);
	
	r=a[0]*a[0]+a[1]*a[1];
	double phi_1 = atan2(a[1],a[0]);

	if( std::abs(r-1.0)>epsilon )           // check length
		return false;

	if( std::abs(phi-phi_1)>epsilon )       // check angle
		return false;

	if( std::abs( a[0]-cos(phi) )>epsilon ) // check real part
		return false;

	if( std::abs( a[1]-sin(phi) )>epsilon ) // check imaginary part
		return false;

	std::cout << "Length after: " << r << std::endl;
	std::cout << "Argument after: " << phi_1 << std::endl;
	std::cout << "Real part: " << a[0] << std::endl;
	std::cout << "Imag part: " << a[1] << std::endl;

	return true;
}
// testing the fftw_add functions
int Test_fftw_normalize(int , char*[])
{
	int ret_ok=0, ret_fail=1;
	fftw_complex a;
/////////////
// Test 1:
	a[0] = 10.0;
	a[1] = 0.0;

	if( !test_normalize(a) )
		return ret_fail;
//////////////
// Test 2:
	a[0] = 123.0;
	a[1] = -123.0;

	if(!test_normalize(a))
		return ret_fail;

//////////////
// Test 3:
	a[0] = a[1] = 0.0;

	// The criteria checked within test_normalize do not apply.
	// The norm is zero and the fftw_normalize should simply return
	// quietly without changing the value of a.

	test_normalize(a);
	
	if( std::abs( a[0]-0.0 )>epsilon  || std::abs( a[1]-0.0 )>epsilon)
		return ret_fail;
/////////////////////////////////////////////////
	return ret_ok;
}








