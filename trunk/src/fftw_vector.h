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
	fftw_vector() : m_x(0L), m_pFwdPlan(),m_pBwdPlan(), m_size(0L),
                   m_idx(0L), m_plan_mode(FFTW_MEASURE),m_Df(1.0), m_Dt(1.0) {};
	virtual ~fftw_vector();
private:
	fftw_vector(const fftw_vector& ): m_x(0L), m_pFwdPlan(),m_pBwdPlan(), m_size(0L),
                                     m_idx(0L), m_plan_mode(FFTW_MEASURE),m_Df(1.0), m_Dt(1.0){}
	fftw_vector& operator=(const fftw_vector& ){return *this;}
public:
	void re_alloc( std::size_t N, bool fftw_estimate);
	void normalize();
	double norm() const;
	void fft();
	void ifft();
	void fft_filter(double bw, double f_s);
	void setSampleTime(double dt);
	bool full(){return m_idx == m_size;}

	std::size_t size() const {return m_size;}
	double frequency(int i) const {return static_cast<double>(i)*m_Df;}
	double time(int i) const {return static_cast<double>(i)*m_Dt;}
	double sampleTime() const {return m_Dt;}
	void write(const std::string& file_name, bool freq=true);

	fftw_complex& operator[](std::size_t i){return m_x[i];}

	// Element-wise complex conjugate multiplication: z = z \cdot \bar{w}. z is *this.
	void mult_conjugate(fftw_vector& p, bool conjugate=false);
	void mult_conjugate(const fftw_complex& p, bool conjugate=false);

	void set_samples(const std::vector<double>& real, const std::vector<double>& imag, bool fftw_estimate);

	template<typename Sample_Type>
		void push_back(const Sample_Type& sample);

	template<typename Sample_Type>
		void set_samples(const std::vector<Sample_Type>& vec, bool fftw_estimate);

protected:
	void free_samples();
	void copy_sample(fftw_complex& z, double x) const;
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

#endif // FFTW_VECTOR_H





