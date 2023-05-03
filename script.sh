#!/bin/sh

dr="instances"

mkdir -p "$dr"

for n in 50 100 200 350
do
	for p in 1 5 10
	do
        for times in A B C D E
        do
            	filename="$dr/${n}_${p}_${times}.txt"
		python3 instance_generator.py $n $p > "$filename"
	done
    done
done
