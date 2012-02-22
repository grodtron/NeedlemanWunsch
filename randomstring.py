#!/usr/bin/python

from random import random

len1 = int(random() * 10) + 20
len2 = int(random() * 10) + 20

s1 = ""
s2 = ""

for n in range(len1):
   s1 += "ACGT"[int(random()*4)]

for n in range(len2):
   s2 += "ACGT"[int(random()*4)]

print s1, s2
