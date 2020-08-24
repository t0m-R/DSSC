CC = gcc
CFLAGS = -O0 -ggdb -fopenmp -std=c11 -I$(IDIR)
EXE = assignment2

SRC = src/loop_schedules.c
IDIR = include


all: $(EXE)


$(EXE): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^
