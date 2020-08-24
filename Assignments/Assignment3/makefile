MPICC = mpicc
MPICFLAGS = -O0 -ggdb -std=c11
CC = gcc
CFLAGS = -O0 -ggdb -fopenmp -std=c11 

MPI = pi_mpi.x
OMP = pi_omp.x

all: $(MPI) $(OMP)

SRC_MPI = src/pi_mpi.c 
SRC_OMP = src/pi_omp.c 

$(MPI): $(SRC_MPI)
	$(MPICC) $(MPICFLAGS) -o $@ $^

$(OMP): $(SRC_OMP)
	$(CC) $(CFLAGS) -o $@ $^

