///////////////////////////////////////////////////////////////////////////////
// Description:
//
///////////////////////////////////////////////////////////////////////////////
// SVN Header (The following information will be automatically updated by SVN)
// $Author: $
// $Date:  $
// $Revision: $
// $Id:$
//////////////////////////////////////////////////////////////////////////////
//Includes: ///////////////////////////////////////////////////////////////////
#include "generator.h"
///////////////////////////////////////////////////////////////////////////////
int main(int , char*[])
{
	boost::mt19937 rng( 1234 );  // on gcc the cast will use the LSBs
	boost::normal_distribution<> gauss_dist;   // Gauß-ian distribution with mean 0 and variance 1
	boost::variate_generator<boost::mt19937&, boost::normal_distribution<> >
           gauss(rng, gauss_dist);             // glues randomness with mapping

	double t=0.0;
	double w = 2.0*3.14159265;
	while(1)
	{
		std::cout << t << ' ' << sin(w*t) + gauss() << std::endl;
		t+=0.1;
		usleep(10000);
	}
}


