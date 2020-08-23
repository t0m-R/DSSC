#!/bin/bash

module load openmpi/1.8.3/gnu/4.9.2

threads=$1;

mkdir build && cd "$_" || exit;
cmake ..;
make;

mpirun -np "$threads" ./assignment4;
