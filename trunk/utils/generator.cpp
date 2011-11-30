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
int main(int argc, char* argv[])
{
	boost::mt19937 rng( 1234 );  // on gcc the cast will use the LSBs
	boost::normal_distribution<> gauss_dist;   // Gauß-ian distribution with mean 0 and variance 1
	boost::variate_generator<boost::mt19937&, boost::normal_distribution<> >
           gauss(rng, gauss_dist);             // glues randomness with mapping

	unsigned int i=0, N=0;
	const double two_pi = 4.0*acos(0.0);
	double t=0.0;
	double w_0 = 3.5*two_pi;

	if(argc == 2)
		N = boost::lexical_cast< unsigned int >(argv[1]);

	while(1)
	{
		std::cout << t << ' ' << sin(w_0*t) + gauss() << std::endl;
		t+=0.1;

		if( (int(t)%10000) >5000 )// Every 5000 point toggle the frequency:
 			w_0 = 3.0*two_pi;
 		else
 			w_0 = 3.5*two_pi;

		if(N==0)
			usleep(200);
		else
			if(N==(++i)) break;
	}
}


