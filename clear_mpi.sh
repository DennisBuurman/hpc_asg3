#!/bin/sh

# Dummy run to clear any errors
for i in {1..24}; do
	prun -np 1 \
	-script $PRUN_ETC/prun-openmpi \
	/home/hpcl2201/projects/hpc1-asg3/psort 100000 \
	&
done