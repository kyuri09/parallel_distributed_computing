#!/usr/bin/env bash

if [ $# -ne 4 ];
then
	echo "Usage) \$sh execution.sh [flip/grayscale/smooth] [2|4|8|...|512] [input_path] [output_path] "
	exit 1;
fi

mpiexec -np $2 -mca btl ^openib -hostfile hosts ./parallel $1 $3 $4

