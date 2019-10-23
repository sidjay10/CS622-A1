#include<stdio.h>
#include<stdlib.h>
#include"cold.h"
#include <assert.h>



typedef unsigned long long ull;

ull Inclusive(char **);
ull lru(int, char**);
ull belady(int, char **);

int main(int argc, char ** argv)
{
	int B = 64;
    
    ull Cold_Miss = 0;
	cold ** hashc;

	unsigned int hszc = 100003;
	hashc = hash_init_cold(hszc);

	char output_name[100];
	unsigned long long Addr;

	FILE* fp;
	sprintf(output_name, "%s_output", argv[1]);
	fp = fopen(output_name, "rb");
	assert(fp != NULL);

	while(!feof(fp))
	{
		fread(&Addr, sizeof(unsigned long long), 1, fp);
		Cold_Miss += hash_cold_insert(hashc,Addr,B,hszc);
	}

	fclose(fp);
	free_hash_cold(hashc,hszc);

	ull L3_Miss_SA = Inclusive(argv);

	ull L3_Miss_FA = lru(2,argv);

	printf("%llu %llu\n",Cold_Miss,L3_Miss_FA);

	L3_Miss_FA = belady(2,argv);

	printf("%llu %llu\n",Cold_Miss,L3_Miss_FA);
}
