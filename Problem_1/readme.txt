CS622 - Assignment1 - 31st August 2019
Group 5
	Siddharth Jayashankar (170699)
	Abir Mukherjee (19111005) 
---------------------------------------
readme.txt : Problem 1

1. To compile the file, run the bash script in compile.sh

2. To execute for a particular trace file, run the bash script in run.sh with the first argument (without the underscore and the number) as the file name and the second argument as the number of trace files.

	eg. $ bash run.sh gcc.log_l1misstrace 2

3. The program will output statistics pertaining of the trace for inclusive, NINE and exclusive caches. The statistics will be available in two variants - an absolute count and per 1K L2 Accesses (PKL2A).