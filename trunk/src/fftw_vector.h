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
#ifndef FFTW_VECTOR_H
#define FFTW_VECTOR_H

// Standard includes: /////////////////////////////////////////////////////////
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>

// Third party includes:  /////////////////////////////////////////////////////
#include <fftw3.h>        // Include Fast Fourier Transform Library

// Project specific includes: /////////////////////////////////////////////////
#include "fftw_utils.h"

// Class declarations: ////////////////////////////////////////////////////////
class fftw_vector
{
public:
   fftw_complex*   m_x;

protected:
   fftw_plan       m_pFwdPlan, m_pBwdPlan;
   std::size_t     m_size;
   std::size_t     m_idx;
   int             m_plan_mode;
   double          m_Df;  // Frequency bin size [Hz]
   double          m_Dt;  // Sample time [s]

public:
   fftw_vector();
   fftw_vector(size_t size, double real, double imag);
   fftw_vector(fftw_vector& fft_vec);

   virtual ~fftw_vector();

public:
   void re_alloc( std::size_t N, bool fftw_estimate = FFTW_ESTIMATE);
   double norm() const;
   double normSquare() const;
   void normalize();
   void normalizeSquare();
   void normalizeToValue(double norm);
   void fft();
   void ifft();
   void acf(bool removeBartlettWindow=false);
   void acf_normalized(bool removeBartlettWindow=false);
   void fft_filter(double bw, double f_s);
   void setSampleTime(double dt);
   void setSampleFrequency(double df);
   bool full(){return m_idx == m_size;}
   void initAll(double real, double imag);

   std::size_t size() const {return m_size;}
   double frequency(int i) const {return static_cast<double>(i)*m_Df;}
   double time(int i) const {return static_cast<double>(i)*m_Dt;}
   double sampleTime() const {return m_Dt;}
   double sampleFrequency() const {return m_Df;}
   polar getPolar(int idx);
   double abs(int idx) const {return sqrt(m_x[idx][0]*m_x[idx][0]+ m_x[idx][1]*m_x[idx][1]);}
   double arg(int idx) const {return atan2(m_x[idx][1],m_x[idx][0]);}
   double real(int idx) const {return m_x[idx][0];}
   double imag(int idx) const {return m_x[idx][1];}
   void getSample(double& x, double& y, int idx, bool polar_coord);

   fftw_complex& operator[](std::size_t i){return m_x[i];}
   fftw_vector& operator=(const fftw_vector& rhs);

   // Element-wise complex conjugate multiplication: z = z \cdot \bar{w}. z is *this.
   // Please note that the parameter conjugate_result refer to the _result_ not to the operation itself.
   void mult_conj(fftw_vector& p, bool conjugate_result=false);
   void mult_conj(const fftw_complex& p, bool conjugate_result=false);

   void set_samples(const std::vector<double>& real, const std::vector<double>& imag, bool fftw_estimate);

   void push_back(const double real, const double imag);

   template<typename Sample_Type>
      void push_back(const Sample_Type& sample);

   template<typename Sample_Type>
      void set_samples(const std::vector<Sample_Type>& vec, bool fftw_estimate);

private:
   void normalizeACF();
   void free_samples();
   void copy_sample(fftw_complex& z, double x) const;
   void copy_sample(fftw_complex& z, const fftw_complex& rhs) const;
   void removeImplicitBartlettWindow();
   void acf_unscaled(bool removeBartlettWindow);
};

///////////////////////////////////////////////////////////////////////////////
/**
 * Small template function to handle the copying of data from various vectors
 * to fftw_complex PODs. 
 */
template<typename Sample_Type>
void fftw_vector::set_samples( const std::vector< Sample_Type >& vec, bool fftw_estimate)
{
   re_alloc( vec.size(), fftw_estimate );

   for(std::size_t i=0; i<m_size; i++)
      copy_sample(m_x[i], vec[i]);
}
///////////////////////////////////////////////////////////////////////////////
template<typename Sample_Type>
void fftw_vector::push_back(const Sample_Type& sample)
{
   if(m_idx<m_size)
   {
      copy_sample(m_x[m_idx], sample);
      m_idx++;
   }
}

///////////////////////////////////////////////////////////////////////////////
inline int findLargestPowerSample(const fftw_vector& vec)
{
   double max_magnitude=0.0;
   double magnitude=0.0;
   int maxIdx = 0;
   for(auto i=0ul; i<vec.size(); i++)
   {
      magnitude =  vec.abs(i);
      if(magnitude>max_magnitude)
      {
         max_magnitude = magnitude;
         maxIdx = i;
      }
   }
   return maxIdx;
}


#endif // FFTW_VECTOR_H





