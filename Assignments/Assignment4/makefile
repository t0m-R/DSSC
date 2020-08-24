MPICC = mpicc
MPICFLAGS = -O0 -ggdb -std=c11
SRC = src

all: ring.x

%.x: $(SRC)/%.c
	$(MPICC) $(MPICCFLAGS) -o $@ $<
