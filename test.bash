#!/bin/bash

function test( ){
   total1=0.0
   total2=0.0
   for n in {1..100}
   do
      result=$(./nw $1 $2)
      total1=$(echo "$(awk '{print $1}' <<< "$result") + $total1" | bc -iq | tail -n1)
      total2=$(echo "$(awk '{print $2}' <<< "$result") + $total2" | bc -iq | tail -n1)
   done
      echo $total1 $total2
}


for iteration in {1..100}
do
   dna=$(./randomstring.py)
   echo "Testing with: $dna" > /dev/stderr
   test $dna
done

