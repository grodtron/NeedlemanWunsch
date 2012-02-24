#!/usr/bin/python

# This script generates fake dna strings that are supposed to
# have some common ground. IMHO using tweets is more interesting

from random import choice, random
from sys import argv

def swap(A, i, j):
   A[i], A[j] = A[j], A[i]

def insert(A, i, l):
   return A[:i] + [choice("ACGT") for n in range(l)] + A[i:]

length = int(argv[1]) if len(argv) == 2 else 40

s = ""
for n in range(length):
   s += choice("ACGT")

a = list(s)
# make mutations
for n in range(length/4):
   swap(a, int(random()*length), int(random()*length))
# make insertions
for n in range(int(random()*length)/10):
   a = insert(a, int(random()*len(a)), int(random()*length/15))

b = list(s)
#make mutations
for n in range(length/4):
   swap(b, int(random()*length), int(random()*length))
# make insertions
for n in range(int(random()*length)/10):
   b = insert(b, int(random()*len(a)), int(random()*length/15))

print ''.join(a)
print ''.join(b)

