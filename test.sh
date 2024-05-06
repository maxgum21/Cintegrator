#!/bin/bash

let mode=0;

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
			./cintegrator -ti $line
		else
			./cintegrator -t $line
		fi
	fi
done < "$1"

