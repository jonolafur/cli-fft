///////////////////////////////////////////////////////////////////////////////
// Description:
// Dependencies (others than those explicitly visible in the #include clause):
//    fftw3.
// Project:
///////////////////////////////////////////////////////////////////////////////
// SNV Header (The following information will be automatically updated by SVN)
// $Author: jon$
// $Date:$
// $Revision:$
// $Id:$

#include "window_function.h"

window_function::window_function() : m_N(0)
{}


window_function::~window_function()
{
}


void window_function::applyWindow(fftw_vector& fft_vec)
{
   setSize( fft_vec.size() );

   double n=0.0;

   for(size_t i=0; i<fft_vec.size(); i++, n += 1.0)
      windowFunction(n, fft_vec[i]);
}


// Implementation Bartlett window:

bartlett::bartlett() : m_alpha(0.0)
{
}


void bartlett::setSize(size_t N)
{
   if(N<2)
      throw "Setting data size in bartlett window failed. Data size must be larger than 2.";

   m_N = static_cast<double>(N);
   m_alpha = 1.0/( 0.5*(m_N-1.0) );
}


void bartlett::windowFunction(double n, fftw_complex& c)
{
   double w = 1.0-std::abs( (n -0.5*(m_N-1.0))*m_alpha );

   c[0] *= w;
   c[1] *= w;
}


// Implementation Hann window:
hann::hann::hann() : m_alpha(0.0)
{
}

void hann::setSize(size_t N)
{
   double pi = 2.0*acos(0.0);
   
   if(N<2)
      throw "Setting data size in bartlett window failed. Data size must be larger than 2.";

   m_N = static_cast<double>(N);
   m_alpha = pi/(m_N-1.0);
}


void hann::windowFunction(double n, fftw_complex& c)
{
   double w = sin( n*m_alpha );

   w*=w;

   c[0] *= w;
   c[1] *= w;
}



