#!/usr/bin/env bash

function ctrl_c() {
        exit
}
trap ctrl_c INT

while true
	do
		sh $1 $2 $3
done
