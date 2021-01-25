#/bin/bash

CELLOPT=~/NetBeansProjects/cellopt/dist/Release/CLang-Linux/cellopt

myres=$1

[ -e ${myres} ] || (echo "File ${myres} does not exist, exiting" && exit )

for i in $(seq -w 20); do
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
