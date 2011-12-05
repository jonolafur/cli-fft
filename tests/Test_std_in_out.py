#!/usr/bin/python
# Since CTest does not support file re-directing we need this
## somewhat awkward wrapper to test the re-directing supported be fft:
import sys, subprocess, os

fft_command = sys.argv[1]+"/fft"
test_file_in=sys.argv[1]+"/tests/Test_read_file.txt"
test_file_out=sys.argv[1]+"/tests/Test_fft_std_in_out.txt"

print(fft_command)

os.path.normpath(fft_command)
os.path.normpath(test_file_in)
os.path.normpath(test_file_out)

print(fft_command)
print(test_file_in)
print(test_file_out)

option_string = fft_command + " -c -x 1 -y 2 -N < " +test_file_in +" > "+ test_file_out

print(option_string)

os.system(option_string)

#subprocess.call([fft_command, " -c -x 1 -y 2 -N < ", test_file_in, " > ", test_file_out ] )

