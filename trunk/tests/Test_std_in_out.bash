#!/bin/bash
# Since CTest does not support file re-directing we need this
## somewhat awkward wrapper to test the re-directing supported be fft:
../fft -c -x 2 -y 3 < test.txt > test_fft.txt