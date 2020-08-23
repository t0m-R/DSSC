#!/bin/bash

module load openmpi/1.8.3/gnu/4.9.2
mkdir build && cd "$_" || exit;
cmake ..;
make;
touch results_mpi.txt;
touch results_omp.txt;

for threads in {1..40..1}
do
export OMP_NUM_THREADS=$threads;
./pi_omp.x >> results_omp.txt
mpirun -np $threads ./pi_mpi.x >> results_mpi.txt
done
