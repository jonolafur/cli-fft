2011-11-05:

1. Overview
This is a small utility for generating FFT of ASCII files or standard in and output to file or std out. The build is static, so no complicated dependecies must be fulfilled and thus it should be possible to use the binary with any distribution that runs on x86 or x86_64.

2. Installing
To install, just copy the binary someplace where the system finds it (e.g. in your path.)

3. Examples of use
The following call to fft

  $> fft < input.txt > output.txt

will consider the first column in the data file input.txt as the real-valued time-series to be transformed.

  $> fft -i input.txt -o output.txt -c

Same as above, except the option "-c" tells fft to interpret the input as complex data. The first and second columns are now interpreted as real and imaginary compomnets of a complex time-series.

  $> fft --input-file input.txt --output-file output.txt \
         --complex -x 3 -y 6 --delimiter , --ordered-x-axis

Using the long options. In this example the data columns are separated by ','.

In this example a x-column is specified. This instructs fft to treat that column as the time coordinate. Currently, all that is done is to calculate the average sample rate from the first and last values in this column and use that to generate a frequency axis for the output.

The "natural" way for fftw write its output is to write the negative frequencies "to the right" of the positive ones (i.e. something like  [0,..,F,-F,..,0]). The option --ordered-x-axis tells fft to start with the negative frequencies (i.e. something like  [-F,..,0,..,F]). The data is retrived from column 6 and 7 and column 3 is used to infer the frequency.

4. Building from Source
These build instructions are for Linux. I'm almost sure this
builds under Windows as well, but I'll first describe it here
when I have verified it.

fft depends on several things, which are listed below:

  a) CMake is the (meta-)build system (www.cmake.org)
  b) fftw is required for the actual FFT'ing (www.fftw.org)
  c) boost for handling program options and other things
     like lexical cast (www.boost.org).
  d) Subversion. You need it to check out the code from sourceforge.
     However, during build a version file is generated that is included
     in the sources. In order to generate this version header, an svn
     is required. It should work with the standard svn client
     installation under Linux, and under Windows TortoiseSVN
     should do the job.
  e) The gcc c++ tool chain 

Talk about standing on the shoulders of giants! Anyway, if you have all these prerequisites set up properly, all you need to do is:

0: Check out the sources by issuing the following command:

  $> svn checkout svn://svn.code.sf.net/p/cli-fft/code/trunk cli-fft

That should leave you with the source tree root at cli-fft (you can of course replace cli-fft with anything you like).

1: cd to the root of the source tree and create a build directory there:
  $> mkdir build
  $> cd build

2: In the build directory issue
  $> cmake ../ -DCMAKE_BUILD_TYPE=release
If there are any issues with the pre-requisites, cmake will tell you about it at this stage. If this goes smoothly, you just have to make:

  $> make

That's it. The binary should now be located in the root of your build tree. You can copy it to somewhere where the system finds it.
 
5. Potential sources of pain

Boost missing: The standard installation of boost is probably not enough. If cmake
complains that it cannot find it, you probably need to install some more boost. Look for libboost-program-options in your package manager.





