#!/bin/bash

module load openmpi/3.1.4/gcc/4.8.5-3wvy3op

N=$1;
threads=$2;

make;

mpirun -np "$threads" ./matrix.x "$N";

