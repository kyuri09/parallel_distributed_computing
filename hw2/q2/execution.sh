#!/usr/bin/env bash

if [ $# -ne 2 ];
then
	echo "Usage) \$sh execution.sh [scan|nonblocking|blocking] [2|4|8|...|512]"
	exit 1;
fi

mpiexec -np $2 -mca btl ^openib -hostfile hosts ./$1

