#!python
from random import choice
from sys import argv
try:
   print ''.join(choice('ACGT') for n in range(int(argv[1])))
   print ''.join(choice('ACGT') for n in range(int(argv[2])))
   print argv[3]
   print argv[4]
   print argv[5]
   print 5
   print 7
except:
   print "Enter the lengths of the two strands as arguments"
