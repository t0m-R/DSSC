#!/bin/bash

module load openmpi/3.1.4/gcc/4.8.5-3wvy3op

threads=$1;

make;

mpirun -np "$threads" ./ring.x;
