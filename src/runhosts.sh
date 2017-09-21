#!/bin/bash


hostport=8000


for i in {1..20..1}
do
	echo $i $hostport
	./host localhost $hostport &
	hostport=$((hostport+1))
done


