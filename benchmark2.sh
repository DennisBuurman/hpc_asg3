#!/bin/bash

if [ $# -lt 1 ]; then
	echo "Usage: ./benchmarks.sh <output file>"
	exit
fi
filepath=$1

arraySizes=(10 1000 100000 10000000 1000000000)
sortMethods=("quick" "stl" "bubble")
seeds=(305441741 270544960 1088997156 2043394589 2893796441)
count=1
maxTime="08:00:00"

true > "${filepath}"
# Benchmark: Trying different internal sorting algorithms
printf "array_size_exponent,algo,rep1,rep2,rep3,rep4,rep5\n" >> "${filepath}"
for method in "${sortMethods[@]}"; do
	for size in "${arraySizes[@]}"; do
		printf "%d,%s," "${size}" "${method}" >> "${filepath}"
		for seed in "${seeds[@]}"; do
			prun -np "${count}" \
			-t "${maxTime}" \
			-script "${PRUN_ETC}/prun-openmpi" \
			/home/hpcl2201/projects/hpc1-asg3/sort "${size}" "${seed}" "${method}"\
			| tr -d '\n'>> "${filepath}"
			printf , >> "${filepath}"
		done
		# Remove trailing comma and move to new line
		truncate -s -1 "${filepath}"
		printf "\n" >> "${filepath}"
	done
done

