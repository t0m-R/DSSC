#!/bin/bash

module load openmpi/1.8.3/gnu/4.9.2 
mkdir build && cd "$_" || exit;
cmake ..;
make;
touch results.txt;

for threads in {1..20..1}
do
export OMP_NUM_THREADS=$threads;
./assignment1 >> results.txt
done
