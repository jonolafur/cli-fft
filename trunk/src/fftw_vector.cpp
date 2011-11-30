///////////////////////////////////////////////////////////////////////////////
// Description:
//   A small wrapper class to handle fftw_complex data: allocating, freeing
//   Prepare ffts, generate and manage plans, etc.
// Dependencies (others than those explicitly visible in the #include clause):
//    fftw3.
///////////////////////////////////////////////////////////////////////////////
// SVN Header (The following information will be automatically updated by CVS)
// $Author: jon $
// $Date:$
// $Revision:$
// $Id:$
///////////////////////////////////////////////////////////////////////////////

#include "fftw_vector.h"
///////////////////////////////////////////////////////////////////////////////
fftw_vector::~fftw_vector()
{
	free_samples();
}
///////////////////////////////////////////////////////////////////////////////
void fftw_vector::re_alloc( std::size_t N, bool fftw_estimate)
{
	std::string fftw_estimation_mode;

	m_idx = 0;   // reset internal index
	
	if(N==0) // If user allocates zero samples, we just free memory and return
	{
		free_samples();
		return;
	}

	if(fftw_estimate)
	{
		m_plan_mode = FFTW_ESTIMATE;
		fftw_estimation_mode = "estimating...";
	}
	else
	{
		m_plan_mode= FFTW_MEASURE;
		fftw_estimation_mode = "measuring, may take longer if wisdom is not available...";
	}
	
	if(m_size != N )
	{
		free_samples();
		m_size = N;
		m_x  = static_cast<fftw_complex*>( fftw_malloc(sizeof(fftw_complex) * N) );
		
		if(m_x==0L)
		{
			std::stringstream ss;
			ss << "Error: Failed to allocate samples for fft. "
			"Number of bytes requested: " << sizeof(fftw_complex)*N << std::endl;
			throw ss.str();
		}
	}

	// This is the right place to generate the fftw plans. We need to do this
	// _before_ we load the data into the array, because during planning this
	// data may be overwritten. Once generated, the plans can be stored in
	// an ASCII file. We will be looking for the file ".fftw_wizdom". If it
	// is available, we try to read the wisdom from it. After we have generated
	// the plans we write the generated wisdom into .fftw_wizdom. The wisdom
	// accumulates in this file.

	// Loading wizdom from file:
	std::string fftw_wizdom_file_name( getenv("HOME") );
	fftw_wizdom_file_name += "/.fftw_wizdom";

	FILE* wis_file = std::fopen(fftw_wizdom_file_name.c_str(), "r");

	if(wis_file == 0)
		std::clog << "Warning: failed to load wisdom. A new wisdom "
                   "file will be generated at:\n" << fftw_wizdom_file_name << '\n';
	else
	{
		std::clog << "Reading wisdom from: " << fftw_wizdom_file_name << '\n';
		fftw_import_wisdom_from_file(wis_file);
		std::fclose(wis_file);
	}

	// Generating plans. We need one for the forward and the backward FFT
	std::clog << "Generating plan for forward transformation..." << fftw_estimation_mode << std::endl;
	m_pFwdPlan  = fftw_plan_dft_1d(N, m_x, m_x, FFTW_FORWARD, m_plan_mode);

	std::clog << "Generating plan for backward transformation..." << fftw_estimation_mode << std::endl;
	m_pBwdPlan = fftw_plan_dft_1d(N, m_x, m_x, FFTW_BACKWARD, m_plan_mode);

	std::clog << "Finsihed generating plans." << std::endl;

	// Write gained wisdom to file for future use:
	wis_file = std::fopen(fftw_wizdom_file_name.c_str(), "w");
	if(wis_file!=0)
	{
		fftw_export_wisdom_to_file(wis_file);
		std::fclose(wis_file);
	}
}
///////////////////////////////////////////////////////////////////////////////
void fftw_vector::fft()
{
	if(m_pFwdPlan)
		fftw_execute(m_pFwdPlan);
	else
		throw "Attempting to perform FFT on invalid plan. Maybe the data vector was empty?";
}
///////////////////////////////////////////////////////////////////////////////
void fftw_vector::ifft()
{
	if(m_pBwdPlan)
		fftw_execute(m_pBwdPlan);
	else
		throw "Attempting to perform IFFT on invalid plan. Maybe the data vector was empty?";
}
///////////////////////////////////////////////////////////////////////////////
void fftw_vector::normalize()
{
	if(m_size == 0)
		return;

	double norm = sqrt(1.0/double(m_size));
	for(std::size_t i=0; i< m_size; ++i)
	{
		m_x[i][0] *= norm;
		m_x[i][1] *= norm;
	}
}
///////////////////////////////////////////////////////////////////////////////
double fftw_vector::norm() const
{
	double norm=0.0;

	for(std::size_t i=0; i< m_size; ++i)
		norm += m_x[i][0]*m_x[i][0] + m_x[i][1]*m_x[i][1];

	return sqrt(norm);
}
///////////////////////////////////////////////////////////////////////////////
polar fftw_vector::getPolar(int idx)
{
	polar p(sqrt(m_x[idx][0]*m_x[idx][0]+ m_x[idx][1]*m_x[idx][1]),
          atan2(m_x[idx][1],m_x[idx][0]));

	return p;
}
///////////////////////////////////////////////////////////////////////////////
void fftw_vector::getSample(double& x, double& y, int idx, bool polar_coord)
{
	if(polar_coord)
	{
		x = abs(idx);
		y = arg(idx);
	}
	else
	{
		x = m_x[idx][0];
		y = m_x[idx][1];
	}
}
///////////////////////////////////////////////////////////////////////////////
void fftw_vector::mult_conjugate(fftw_vector& p, bool conjugate)
{
	if(size() != p.size())
		throw "Unequal sizes in fftw_vector::mult_conjugate(...)";

	// We need to keep track of z[0] because it gets overwritten in the first
	// line and is needed in the second. In case the function is called like:
	// fftw_mult_conj(q,q), p[0] would get overwritten in the first line as well.

	double c = 1.0;
	if(conjugate) c = -1.0;

	for(size_t i=0; i< size(); ++i)
	{
		double z0 = m_x[i][0];
		double p0 =   p[i][0];

		m_x[i][0] = p[i][0]*m_x[i][0] +p[i][1]*m_x[i][1];
		m_x[i][1] = c*(p0*m_x[i][1] -p[i][1]*z0);
	}
}
///////////////////////////////////////////////////////////////////////////////
void fftw_vector::mult_conjugate(const fftw_complex& p, bool conjugate)
{
	double c = 1.0;
	if(conjugate) c = -1.0;

	for(size_t i=0; i< size(); ++i)
	{
		double z0 = m_x[i][0];

		m_x[i][0] = p[0]*m_x[i][0] +p[1]*m_x[i][1];
		m_x[i][1] = c*(p[0]*m_x[i][1] -p[1]*z0);
	}

}
///////////////////////////////////////////////////////////////////////////////
void fftw_vector::setSampleTime(double dt)
{
	if(dt < std::numeric_limits<double>::epsilon() )
		throw "fftw_vector::setSampleTime: too small sample time!";

	if(size() == 0 )
		throw "fftw_vector::setSampleTime: Sample vector is empty. Cannot set sample size.";

	m_Dt = dt;
	m_Df = 1.0/( m_Dt*static_cast<double>(size()) );
}
///////////////////////////////////////////////////////////////////////////////
void fftw_vector::setSampleFrequency(double df)
{
	if(df < std::numeric_limits<double>::epsilon() )
		throw "fftw_vector::setSampleFrequency: too small sample frequency!";

	if(size() == 0 )
		throw "fftw_vector::setSampleFrequency: Sample vector is empty. Cannot set sample frequency.";

	m_Df = df;
	m_Dt = 1.0/( m_Df*static_cast<double>(size()) );
}
///////////////////////////////////////////////////////////////////////////////
void fftw_vector::fft_filter(double bw, double f_s)
{
	if(bw==0.0)
	{
		std::clog << "Specified RF bandwidth was zero. Samples remain un-filtered." << std::endl;
		return;
	}

	// The signal is assumed to be symmetric at base-band. This means that
	// the positive part of the spectrum is located from 0 to m_rf_bw/2 and the
	// negative part is located from (sample_rate-m_rf_bw/2).
	// It can now happen that he requested filtering exceeds the Nyquist
	// frequency. That will result in a start index for the samples to be set to
	// zero that is larger than half the number of samples. This case has to be
	// caught
	// ^
	// |*                                     *
	// |***    *                        *   ***
	// |****  ***  *                *  *** ****
	// |*************             *************
	// |----------------------------------------->  ASCII Art Rocks!
	//                     ^- Nyquist

	std::size_t N = int( static_cast<double>(m_size)*bw/(2.0*f_s) );

	if(N >= m_size/2)
	{
		std::clog << "Warning: Specified RF bandwidth exceeds the Nyquist bandwidth. Samples remain un-filtered." << std::endl;
		return;
	}

	for(std::size_t i=N; i<m_size-N; ++i)
		m_x[i][0] = m_x[i][1] = 0.0;

	std::clog << "First sample index deleted: " << N << '\n'
	          << "Last sample index deleted: " << m_size-N-1 << std::endl;
}
///////////////////////////////////////////////////////////////////////////////
/**
* Applying a frequency response.
* The user can supply a frequency response in an ASCII file. The ASCII data is
* loaded into interpolation objects that are used to provide an interpolated
* value of the frequency response in magnitude and phase. Here this frequency
* Response is applied. The function assumes that the samples to transform are
* located in m_pRotSamples.
*/
///////////////////////////////////////////////////////////////////////////////
void fftw_vector::free_samples()
{
	if(m_x !=0 && m_size !=0)
		fftw_free(m_x);
	m_x = 0;
	m_size = 0;
}
///////////////////////////////////////////////////////////////////////////////
/**
 * These following two small functions handle the type-specific copying implied
 * in the template function correlator::setSamples(...). See definition in the
 * corresponding header file.
*/
void fftw_vector::copy_sample(fftw_complex& z, double x) const
{
	z[0] = x;
	z[1] = 0.0;
}
///////////////////////////////////////////////////////////////////////////////
void fftw_vector::write(const std::string& file_name, bool freq)
{
	std::ofstream out(file_name.c_str());
	std::string comment("# Frequency    Real    Imaginary");
	double Delta=m_Df;

	if(!freq)  // If the vector content is to be interpreted as time domain data
	{
		comment = "# Time    Real    Imaginary";
		Delta=m_Dt;
	}

	out << std::setprecision(12);
	for(std::size_t i=0; i<size(); ++i)
	    out <<  double(i)*Delta  << ' '
            << m_x[i][0] << ' ' << m_x[i][1] << '\n';
}
///////////////////////////////////////////////////////////////////////////////
void fftw_vector::set_samples(const std::vector<double>& real,
		const std::vector<double>& imag, bool fftw_estimate)
{
	if(imag.size()!= real.size())
		throw "Error: Size-mismatch in set samples of fftw vector.";

	std::size_t N = real.size();
	re_alloc( N, fftw_estimate );

	for(std::size_t i=0; i<m_size; i++)
	{
		m_x[i][0] = real[i];
		m_x[i][1] = imag[i];
	}
}



