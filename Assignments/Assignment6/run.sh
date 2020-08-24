#!/bin/sh

module load cuda/10.0.130/gcc/4.8.5-sadyf2g

> output.dat

make -B

for i in 64 256 512 1024 
do
./transpose.x $i >> output.dat
done
