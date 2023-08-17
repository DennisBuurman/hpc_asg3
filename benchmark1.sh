#!/bin/bash

if [ $# -lt 1 ]; then
	echo "Usage: ./benchmarks.sh <output file>"
	exit
fi

filepath=$1
arraySizes=(1000 200000 1600000 80000000 8000000000)
seeds=(305441741 270544960 1088997156 2043394589 2893796441)
nodeCounts=(1 2 4 8 16)
maxTime="08:00:00"

true > "${filepath}"
# Benchmark: scaling on different array sizes
printf "array_size_exponent,num_nodes,rep1,rep2,rep3,rep4,rep5\n" >> "${filepath}"
for count in "${nodeCounts[@]}"; do
	for size in "${arraySizes[@]}"; do
		printf "%d,%d," "${size}" "${count}" >> "${filepath}"
		for seed in "${seeds[@]}"; do
			prun -np "${count}" \
			-t "${maxTime}" \
			-script "$PRUN_ETC/prun-openmpi" \
			/home/hpcl2201/projects/hpc1-asg3/sort "${size}" "${seed}" \
			| tr -d '\n'>> "${filepath}"
			printf , >> "${filepath}"
		done
		# Remove trailing comma and move to new line
		truncate -s -1 "${filepath}"
		printf "\n" >> "${filepath}"
	done
done


