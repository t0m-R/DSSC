#!/bin/bash

module load openmpi/1.8.3/gnu/4.9.2

N=$1;
threads=$2;

mkdir build && cd "$_" || exit;
cmake ..;
make;

mpirun -np "$threads" ./assignment5 "$N";

