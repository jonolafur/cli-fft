#!/usr/local/bin/gnuplot -persist
#
#    
#    	G N U P L O T
#    	Version 4.5 patchlevel 0
#    	last modified 2011-08-09 
#    	System: Linux 3.0.0-12-generic
#    
#    	Copyright (C) 1986-1993, 1998, 2004, 2007-2010
#    	Thomas Williams, Colin Kelley and many others
#    
#    	gnuplot home:     http://www.gnuplot.info
#    	mailing list:     gnuplot-beta@lists.sourceforge.net
#    	faq, bugs, etc:   type "help seeking-assistance"
#    	immediate help:   type "help"
#    	plot window:      hit 'h'
# set terminal x11 
# set output

set nologscale y
set grid
set yrange [1.0:4096]
set xrange [0:6]
plot '< tail -n 4096 test.txt | fft -x 1 -y 2 -p -M' using 1:2

pause 1
reread

#    EOF
