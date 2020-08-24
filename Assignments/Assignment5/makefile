MPICC = mpicc
MPICFLAGS = -O0 -ggdb -std=c11
SRC = src

all: matrix.x

%.x: $(SRC)/%.c
	$(MPICC) $(MPICCFLAGS) -o $@ $<
