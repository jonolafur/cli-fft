#include <iostream>
#include <fstream>
#include <cmath>
///////////////////////////////////////////////////////////////////////////////
// Main test function:
int Test_gen_data( int , char*[] )
{
	int N = 200;
	double eps = 1.0e-8;
	std::ofstream ref_file("ref_file.txt");
	std::ofstream test_file_no_error("test_file_no_error.txt");
	std::ofstream test_file_w_error("test_file_w_error.txt");

	ref_file << "# adding a comment"<< std::endl;
	test_file_no_error << "# adding a comment"<< std::endl << "# and another additional line" << std::endl;

	ref_file.precision(15);
	test_file_no_error.precision(15);
	test_file_w_error.precision(15);

	for(int i=0; i<N; i++)
	{
		double x= double(i)*0.1;
		double a = sin(x);
		double b = cos(x);
		double c = a*a;
		double d = b*b;
		ref_file << a << ' ' << b << ' ' <<  c << " asdjk " << d;
		test_file_no_error << a << ' ' << b << ' ' << c << " asdjk " << d;
		test_file_w_error << a +x*eps << ' ' << b +x*eps << ' '
		<< c+x*eps << " asdjk " << d+x*eps;
		if(i==123)  // Add a random comment in the middle of the file.
		{
			test_file_no_error << "\n# Ha ha... NOBODY expectes the Spanish inquisition!";
			test_file_w_error <<  "\n# Ha ha... NOBODY expectes the Spanish inquisition!";
		}
		if(i<N-1)
		{
			ref_file << std::endl;
			test_file_no_error << std::endl;
			test_file_w_error << std::endl;
		}
	}
	// Add some trailing carrage returns to the error free file:
	test_file_no_error << std::endl << std::endl << std::endl;
	std::cout << "Generates three test files: one for reference, one without errors and one with errors" << std::endl;
	std::cout << "There are comments in the files with the comment character \'#\'" << std::endl;
	std::cout << "There is also a random comment in the middle of the file." << std::endl;
	std::cout << "All files have no trailing carrage returns, except test_file_no_error.txt, which has three trailing cr." << std::endl;

	return 0;
}
///////////////////////////////////////////////////////////////////////////////

























