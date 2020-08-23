#!/bin/bash

module load openmpi/1.8.3/gnu/4.9.2
mkdir build && cd "$_" || exit;
cmake ..;
make;
touch results.txt;
export OMP_NUM_THREADS=10;
./assignment2 >> results.txt
