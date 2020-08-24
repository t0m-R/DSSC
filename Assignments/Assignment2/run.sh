#!/bin/bash

module load openmpi/3.1.4/gcc/4.8.5-3wvy3op
make;
touch results.txt;
export OMP_NUM_THREADS=10;
./assignment2 >> results.txt
