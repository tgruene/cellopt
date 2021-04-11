#!/usr/bin/gnuplot -persist
#
#    
#    	G N U P L O T
#    	Version 5.4 patchlevel 1    last modified 2020-12-01 
#    
#    	Copyright (C) 1986-1993, 1998, 2004, 2007-2020
#    	Thomas Williams, Colin Kelley and many others
#    
#    	gnuplot home:     http://www.gnuplot.info
#    	faq, bugs, etc:   type "help FAQ"
#    	immediate help:   type "help"  (plot window: hit 'h')
# set terminal qt 0 font "Sans,9"
# set output
set term pdfcairo enh color font "Tex Gyre Heros,20"
set encoding utf8

set title "Oseltamivir"
proot= "oseltamivir"

# ls 1 for orig
set style line 1 lw 2 lc "dark-blue"
# ls 1 for optimised values
set style line 2 lw 2 lc "dark-orange"

## Last datafile plotted: "data.txt"
set xlabel "cellopt + shelxl iteration"

set ylabel "[\U+212B]"
set ytics format "%5.4f"

set output sprintf("%s-a.pdf", proot)
plot [0:53] 'cells.txt' usi 0:2 w lp ls 1 ti "a", 23.68505 w l ls 2
set output sprintf("%s-b.pdf", proot)
plot [0:53] 'cells.txt' usi 0:3 w lp ls 1 ti "b", 24.36204 w l ls 2
set output sprintf("%s-c.pdf", proot)
plot [0:53] 'cells.txt' usi 0:4 w lp ls 1 ti "c", 7.41510 w l ls 2

set ylabel "R1(strong)"
set y2label "R1(all)"
set ytics format "%5.4f" nomirror
set y2tics format "%5.4f" nomirror
# set yrange [0.121:0.122]
# set y2range [0.160:0.161]
set key auto
set output sprintf("%s-R1.pdf", proot)
plot [0:30] 'r1.data' usi 1:2 axes x1y1 w lp ls 1 ti  "R1 (strong)", \
	'r1.data' usi 1:3 axes x1y2 w lp  ls 2 ti "R1 (all)"

unset yrange
unset xrange
unset y2label
set ytics auto format "%3.1f"
unset y2tics
unset ylabel
set xlabel "bond number"
set output sprintf("%s-Zbonds.pdf", proot)
plot 'xd_oseltamivir-opt-noH_bonds.tsv' usi 0:9 w lp ls 1 ti "Z-score bonds optimised", \
     'xd_oseltamivir-orig-noH_bonds.tsv' usi 0:9 w lp ls 2 ti "Z-score bonds orig", \
     '../opt-xds/xt_opt_osel_a_bonds.tsv' usi 0:9 w lp ls 3 ti "Z-score bonds reprocessed"

set xlabel "angle number"
set output sprintf("%s-Zangles.pdf", proot)
plot 'xd_oseltamivir-opt-noH_angles.tsv' usi 0:10 w lp ls 1 ti "Z-score angles optimised", \
     'xd_oseltamivir-orig-noH_angles.tsv' usi 0:10 w lp ls 2 ti "Z-score angles orig", \
     '../opt-xds/xt_opt_osel_a_angles.tsv' usi 0:10 w lp ls 3 ti "Z-score angles reprocessed"
