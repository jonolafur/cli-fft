///////////////////////////////////////////////////////////////////////////////
// Description:
//   A small wrapper class to handle fftw_complex data: allocating, freeing
//   Prepare ffts, generate and manage plans, etc.
// Dependencies (others than those explicitly visible in the #include clause):
//    fftw3.
// Project:
///////////////////////////////////////////////////////////////////////////////
// SNV Header (The following information will be automatically updated by SVN)
// $Author: jon$
// $Date:$
// $Revision:$
// $Id:$
// include guards: ////////////////////////////////////////////////////////////
#ifndef WINDOW_FUNCTION_H
#define WINDOW_FUNCTION_H

#include "fftw_vector.h"

// Class declarations: ////////////////////////////////////////////////////////
class window_function
{
protected:
   double m_N;
public:
   window_function();
   virtual ~window_function();
   
   void applyWindow(fftw_vector& fft_vec);
   
protected:
   virtual void setSize(size_t N) = 0;
   virtual void windowFunction(double n, fftw_complex& c) = 0;   
};


class bartlett : public window_function
{
protected:
   double m_alpha;

public:
   bartlett();

protected:
   virtual void setSize(size_t N);
   virtual void windowFunction(double n, fftw_complex& c);
};


class hann : public window_function
{
protected:
   double m_alpha;

public:
   hann();
   
protected:
   virtual void setSize(size_t N);
   virtual void windowFunction(double n, fftw_complex& c);
};


#endif // WINDOW_FUNCTION_H
   
