#!/bin/bash
# Since CTest does not support file re-directing we need this
## somewhat awkward wrapper to test the re-directing supported be fft:
../fft -c -x 1 -y 2 -N < Test_read_file.txt > Test_fft_std_in_out.txt