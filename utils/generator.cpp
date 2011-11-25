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
	double w_0 = 3.5*2.0*3.14159265;
//	double dw = 0.0;
	while(1)
	{
		std::cout << t << ' ' << sin(w_0*t) + gauss() << std::endl;
		t+=0.1;

//		w_0+=0.0001;
 		if( (int(t)%10000) >5000 )
 			w_0 = 3.0*2.0*3.14159265;
 		else
 			w_0 = 3.5*2.0*3.14159265;


// 		{
// 			std::cout << "# w_0: " << w_0 << " t: " << t<< std::endl;
// 			w_0 = 0.0;
// 			std::cout << "# w_0: " << w_0 << " t: " << t<< std::endl;
// 		}

		usleep(200);
	}
}


