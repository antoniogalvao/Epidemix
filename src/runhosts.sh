#!/bin/bash


hostport=7300


for i in {1..100..1}
do
	echo $i $hostport
	./host localhost $hostport &
	hostport=$((hostport+1))
done


