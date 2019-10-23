#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<string.h>
#include"cache.h"




int main(int argc , char ** argv){
	char fileName[100];
	sprintf(fileName,"%s_output",argv[1]);

	printf("Inclusive\n");
	Inclusive(argv);
	printf("NINE\n");
	NINE(argv);
	printf("Exclusive\n");
	Exclusive(argv);
	
	return 0;	
}

