# print some random sequences
python -c \
"
from random import choice
print ''.join(choice('ACGT') for n in range($1))
print ''.join(choice('ACGT') for n in range($2))
"\
> tmp.dat

# input match, gap, mismatch
echo -e "$3\n$4\n$5" >> tmp.dat
# select to print out all alignments
echo    5            >> tmp.dat
# select to print out AN alignment
#echo     4            >> tmp.dat
# quit
echo    7            >> tmp.dat

# run the program and only keep the good output lines
./nw < tmp.dat | sed -nr '/^[ACGT\-]*$/ p'

rm -f tmp.dat
