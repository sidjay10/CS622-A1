#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<string.h>
#include"cache_fn.h"

int main(int argc , char ** argv){
	char fileName[100];
	sprintf(fileName,"%s_output",argv[1]);
	Inclusive(argv);
	return 0;	
}

