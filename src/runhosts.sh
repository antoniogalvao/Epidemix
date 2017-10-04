#!/bin/bash


hostport=8100


for i in {1..50..1}
do
	echo $i $hostport
	./host localhost $hostport &
	hostport=$((hostport+1))
done


