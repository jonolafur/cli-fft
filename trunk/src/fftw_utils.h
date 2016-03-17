///////////////////////////////////////////////////////////////////////////////
// Description:
//  Tiny collection of functions to make life easier handling fftw structures.
// Dependencies (others than those explicitly visible in the #include clause):
//    none
///////////////////////////////////////////////////////////////////////////////
// SVN Header (The following information will be automatically updated by CVS)
// $Author: jon $
// $Date: $
// $Revision: $
// $Id: $
///////////////////////////////////////////////////////////////////////////////
// include guards: ////////////////////////////////////////////////////////////
#ifndef FFTW_UTILS_H
#define FFTW_UTILS_H

// Standard includes: /////////////////////////////////////////////////////////
#include <cmath>
#include <limits>

// Third party includes:  /////////////////////////////////////////////////////
#include <fftw3.h>        // Include Fast Fourier Transform Library

struct polar
{
   polar(double abs, double arg) : r(abs), phi(arg){};
   double r, phi;
};


inline void fftw_mult(fftw_complex& result, fftw_complex p,fftw_complex q )
{
   result[0] = p[0]*q[0] -p[1]*q[1];
   result[1] = p[0]*q[1] +p[1]*q[0];
}


inline void fftw_mult(fftw_complex& z, fftw_complex p)
{
   // We need to keep track of z[0] because it gets overwritten in the first
   // line and is needed in the second. In case the function is called like:
   // fftw_mult_conj(q,q), p[0] would get overwritten in the first line as well.
   double z0=z[0];
   double p0=p[0];
   z[0] = p[0]*z[0] -p[1]*z[1];
   z[1] = p0*z[1] +p[1]*z0;
}


///////////////////////////////////////////////////////////////////////////////
///Returns the multiple of p times the complex conjugate of q: result = p*conj(q)
inline void fftw_mult_conj(fftw_complex& result, fftw_complex p,fftw_complex q )
{
   result[0] =  p[0]*q[0] +p[1]*q[1];
   result[1] = -p[0]*q[1] +p[1]*q[0];
}


///////////////////////////////////////////////////////////////////////////////
///Replaces z with z times the complex conjugate of p: z -> z*conj(p)
inline void fftw_mult_conj(fftw_complex& z, fftw_complex p)
{
   // We need to keep track of z[0] because it gets overwritten in the first
   // line and is needed in the second. In case the function is called like:
   // fftw_mult_conj(q,q), p[0] would get overwritten in the first line as well.
   double z0=z[0];
   double p0=p[0];
   z[0] = p[0]*z[0] +p[1]*z[1];
   z[1] = p0*z[1] -p[1]*z0;
}


inline void fftw_add(fftw_complex& z, fftw_complex p)
{
   z[0] += p[0];
   z[1] += p[1];
}


inline void fftw_add(fftw_complex& z, double p)
{
   z[0] += p;
}


inline void fftw_add(fftw_complex& z, fftw_complex p, fftw_complex q )
{
   z[0] = p[0]+q[0];
   z[1] = p[1]+q[1];
}


inline void add_polar(double& r, double& p, double r1, double p1)
{
   double x=r*cos(p);
   double y=r*sin(p);
   
   x+=r1*cos(p1);
   y+=r1*sin(p1);
   
   r=sqrt(x*x+y*y);
   p=atan2(y,x);
}


inline void fftw_normalize(fftw_complex& z)
{
   double norm = sqrt(z[0]*z[0]+z[1]*z[1]);

   if( norm < 1e-100 )
      return;

   z[0] /= norm;
   z[1] /= norm;
}

#endif // FFTW_UTILS_H





