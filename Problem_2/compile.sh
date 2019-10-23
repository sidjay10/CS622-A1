#!/bin/bash

gcc opt.c -o opt.o
gcc cache.c cache_fn.c -o cache.o
gcc lru.c lru_fn.o cache_fn.o cold.c -o lru.o
gcc belady.c belady_fn.o cache_fn.o cold.c -o belady.o