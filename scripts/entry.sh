#!/usr/bin/env bash

if [[ "$1" -eq "1" ]] ; then
	build/tsp
elif [[ "$1" -eq "2" ]] ; then
	build/tsp_v2
fi

