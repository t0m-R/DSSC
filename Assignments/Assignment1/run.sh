#!/bin/bash

module load openmpi/3.1.4/gcc/4.8.5-3wvy3op 
make;
touch results.txt;

for threads in {1..20..1}
do
export OMP_NUM_THREADS=$threads;
./assignment1 >> results.txt
done
