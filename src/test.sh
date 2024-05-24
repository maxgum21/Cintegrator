#!/bin/bash

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
			./cintegrator $2 $3 -ti $line
		else
			./cintegrator $2 $3 -t $line
		fi
	fi
done < "$1"

