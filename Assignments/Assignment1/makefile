CC = gcc
CFLAGS = -O0 -ggdb -fopenmp -std=c11 -I$(IDIR)
EXE = assignment1

SRC = src/pi.c
IDIR = include


all: $(EXE)


$(EXE): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^
