CS622 - Assignment1 - 31st August 2019
Group 5
	Siddharth Jayashankar (170699)
	Abir Mukherjee (19111005) 
---------------------------------------
readme.txt : Problem 2

1. To compile the file, run the bash script in compile.sh

2. To execute for a particular trace file, run the bash script in run.sh with the first argument (without the underscore and the number) as the file name and the second argument as the number of trace files.

	eg. $ bash run.sh gcc.log_l1misstrace 2

3.1 The program will output statistics of the Set Associative LRU, Fully Associative LRU and Fully Associative Belady L3 cache. The statistics will be available in two variants - an absolute count and per 1K L2 Accesses (PKL2A).

3.2 The program will also output the counts of Cold Misses, Capacity Misses and Conflict Misses of the Set Associative LRU for the case when zero conflict misses are defined as the Fully Associative LRU and Fully Associative Belady.