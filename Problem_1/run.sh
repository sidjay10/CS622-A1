#!/bin/bash

./opt.o $1 $2

{ echo '-' ; ./cache.o $1; } | column -t

rm "$1_output"