#include "fftw_utils.h"
#include <iostream>

bool test_add(fftw_complex a, fftw_complex b, fftw_complex truth)
{
	double epsilon=1e-12;
	fftw_complex z;

	double r_a=sqrt(a[0]*a[0]+a[1]*a[1]);
	double r_b=sqrt(b[0]*b[0]+b[1]*b[1]);
	
	double phi_a = atan2(a[1],a[0]);
	double phi_b = atan2(b[1],b[0]);

/////////////////////////////////////////////////
	
	add_polar(r_a,phi_a,r_b,phi_b);
	fftw_add(z,a,b);
	
	std::cout << "a=(" << a[0] << ',' << a[1] << ")\n";
	std::cout << "b=(" << b[0] << ',' << b[1] << ")\n";
	std::cout << "polar_a=(" << r_a << ',' << phi_a << ")\n";
	std::cout << "polar_b=(" << r_b << ',' << phi_b << ")\n";

	std::cout << "z=a+b=(" << z[0] << ',' << z[1] << ")\n";
	std::cout << "polar_a+=polar_b(" << r_a << ',' << phi_a << ")\n";
	std::cout << "in Cartesian coords: (" << r_a*cos(phi_a) << ',' << r_a*sin(phi_a) << ")\n";
	
	if(std::abs(z[0]-truth[0])>epsilon || std::abs(z[1]-truth[1])>epsilon)
		return false;

	if(std::abs(r_a*cos(phi_a)-truth[0])>epsilon || std::abs(r_a*sin(phi_a)-truth[1])>epsilon)
		return false;
	
	return true;
}
// testing the fftw_add functions
int Test_fftw_add(int , char*[])
{
	int ret_ok=0, ret_fail=1;
	fftw_complex a, b, truth;
/////////////
// Test 1:
	a[0] = 1.0;
	a[1] = 1.0;

	b[0] = 1.0;
	b[1] = 1.0;

	truth[0]=2.0;
	truth[1]=2.0;

	if(!test_add(a,b,truth))
		return ret_fail;
//////////////
// Test 2:
	a[0] = 1.0;
	a[1] = 1.0;

	b[0] = -1.0;
	b[1] = 1.0;

	truth[0]=0.0;
	truth[1]=2.0;

	if(!test_add(a,b,truth))
		return ret_fail;
	

/////////////////////////////////////////////////
	return ret_ok;
}








