#! /usr/bin/env bash

prefix=testcase/trans
rm result
for file in $prefix*; do
	printf "testcase $file fifo " >> result
	./fifo < "${file}" >> result
	echo "" >> result

	printf "testcase $file nosplit " >> result
	./nosplit < "${file}" >> result
	echo "" >> result

	printf "testcase $file oldest " >> result
	./oldest < "${file}" >> result
	echo "" >> result
	
	for n in 1 2 3 4 5 6 7 8 9 10 
	do
		sleep 1.1
		printf "testcase $file random/$n " >> result
		./random < "${file}" >> result
		echo "" >> result
	done

	printf "testcase $file dtsize " >> result
	./dtsize < "${file}" >> result
	echo "" >> result

	printf "testcase $file dp " >> result
	./dp < "${file}" >> result
	echo "" >> result

	printf "testcase $file constchunk " >> result
	./constchunk < "${file}" >> result
	echo "" >> result

	printf "testcase $file agesum " >> result
	./agesum < "${file}" >> result
	echo "" >> result

done
