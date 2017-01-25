#!/bin/bash
KB=1024
MB=$((1024 * 1024))
SIZES=( 
	$((512))
	$((1 * $KB))
	$((4 * $KB))
	$((8 * $KB))
	$((16 * $KB))
	$((32 * $KB))  
	$((1 * $MB))
	$((2 * $MB))
	$((4 * $MB))
)

INPUT_FILE="edges.csv"

printf "===========================
Testing write_blocks_seq
===========================\n"

for SIZE in "${SIZES[@]}"
	do
		printf "Current size is $SIZE bytes\n";
		./write_blocks_seq $INPUT_FILE $SIZE
		printf "\n"
	done
