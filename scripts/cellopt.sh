#/bin/bash

if (( $# != 1)); then
	echo "res-file missing"
	echo "Usage: cellopt.sh myfile.res"
	echo " make sure, the crystal system is correct (option '-x')"
	exit
fi

CELLOPT=~/NetBeansProjects/cellopt/dist/Release/CLang-Linux/cellopt

[ -e ${CELLOPT} ] || (echo "Executable ${CELLOPT} does not exist, exiting" && exit )

myres=$1

[ -e ${myres} ] || (echo "File ${myres} does not exist, exiting" && exit )


rm mycellopt.log
rm r1.data
rm cells.txt

/bin/echo -n "00 " > r1.data
grep "^REM R1 =" ${myres} | cut -c10-16,43-49 >> r1.data
grep "^CELL " ${myres} |  cut -d'L' -f3 | tee -a cells.txt

for i in $(seq -w 99); do
readarray -t NCELL < <(${CELLOPT} -f ${myres} -v0 -xa)
# move old cell out of 
sed -i "s/^CELL \(.*\)/REM CELL \1\n${NCELL[0]}/" ${myres}
mv ${myres} ${myres%res}ins
shelxl ${myres%.res}
echo "Run $i: ${NCELL[1]}" >> mycellopt.log
echo "Run $i: ${NCELL[0]}" >> mycellopt.log
/bin/echo -ne "Run $i: " >> mycellopt.log
grep "REM R1 = " ${myres}  >> mycellopt.log
done


# the 'cuts' should be fine for 10-99 cycles
grep "R1 =" mycellopt.log | cut -c4-6,17-24,51-57 | tee -a r1.data
grep CELL mycellopt.log| cut -d'L' -f3 | tee -a cells.txt

