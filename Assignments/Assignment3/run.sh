#!/bin/bash

module load openmpi/3.1.4/gcc/4.8.5-3wvy3op
make;

touch results_mpi.txt;
touch results_omp.txt;

for threads in {1..40..1}
do
export OMP_NUM_THREADS=$threads;
./pi_omp.x >> results_omp.txt
mpirun -np $threads ./pi_mpi.x >> results_mpi.txt
done
