#!/bin/bash

if [[ $1 =~ .*trace$ ]]; then
	./opt.o $1 $2;
	touch tmp.txt;

	echo "";
	echo "Set Associative LRU";
	echo '-';
	./cache.o $1 > tmp.txt;
	cat tmp.txt;
	b=$(cat tmp.txt | grep L3_Miss | cut -f2);
	echo "";
	echo "Fully Associative LRU";
	echo '-';
	./lru.o $1 > tmp.txt;
	c=$(cat tmp.txt | grep Cold_Miss | cut -f2);
	cat tmp.txt | tee >( a=$( grep Cap_Miss | cut -f2 ) ; echo "Conf_Miss_SA    "$((b-a-c)); echo '-' ;  echo ""; echo "Fully Associative Belady"; echo '-' ;);
	./belady.o $1 > tmp.txt;
	c=$(cat tmp.txt | grep Cold_Miss | cut -f2);
	cat tmp.txt | tee >( a=$( grep Cap_Miss | cut -f2 ) ; echo "Conf_Miss_SA    "$((b-a-c)); echo '-' ; );

	rm $1_output;
	rm tmp.txt;
else
	echo "Incorrect Input "$1 
fi


