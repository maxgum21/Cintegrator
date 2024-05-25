#!/bin/bash

prog=$1
let mode=0;
echo "$1 $2 $3"
while read -r line; do
	
	if [ "$line" = "area:" ]
	then
		mode=1
	elif [ "$line" = "intersection:" ]
	then 
		mode=0
	else 
		if [ "$mode" -eq "1" ]
		then
			./$prog $3 $4 -ti $line
		else
			./$prog $3 $4 -t $line
		fi
	fi
done < "$2"

