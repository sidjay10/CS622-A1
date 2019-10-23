#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<string.h>
#include"cache_fn.h"
#include"belady_fn.h"
#include"cold.h"

int main(int argc, char ** argv)
{
	cache **L2;
	L2 = init(L2B,L2R,L2A);
	typ A3 = 32768; //Associativity of L3 - must be a power of 2

	chp **heap3;
	heap3 = heap_init(A3);

	chs **hash3;
	typ hsz3 = 4999;

	hash3 = hash_init(hsz3);
	int B = 64;

	bel ** hashb;
	typ hszb = 100003;

	hashb = hash_init_bel(hszb);

	ull Time = 0;
	ull L2_Miss = 0;
	ull L2_Hit = 0;
	ull L3_Miss = 0;
	ull L3_Hit = 0;
    
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
		Time++;
		hash_bel_insert(hashb,Addr,Time,B,hszb);
	}

	fclose(fp);

	fp = fopen(output_name, "rb");
	assert(fp != NULL);

	ull I_count = 0;

	while (!feof(fp)) {
		fread(&Addr, sizeof(unsigned long long), 1, fp);
		I_count++;

		hash_bel_update(hashb,Addr,B,hszb);

		Time = hash_bel_search(hashb,Addr,B,hszb);


		chs *p3;

		if(Hit_Miss(L2,L2R,L2A,L2B,Addr,I_count))
		{

			if((p3=search_htbl(hash3,(Addr/B),hsz3))!=NULL)
				hit(heap3,A3,p3->hp,Time);

			L2_Hit++;
		}
		else
		{
			L2_Miss++;
            
			

			if((p3=search_htbl(hash3, Addr/B, hsz3))!=NULL)
			{
				L3_Hit++;
				hit(heap3,A3,p3->hp,Time);
				
			}

			else
			{
				L3_Miss++;
                Cold_Miss += hash_cold_insert(hashc,Addr,B,hszc);
				ull evi3 = miss(heap3,A3,B,Addr,Time,hash3,hsz3);
				if(evi3!=-1)
				{
					;
					Invalidate(L2,L2B,L2R,L2A,evi3*B);
				}
				
			}
			Miss_Handler(L2,L2B,L2R,L2A,Addr,I_count);
		}
	}
	fclose(fp);
	Time = I_count;

    ull Cap_Miss = L3_Miss-Cold_Miss;

	//printf("Fully_Associative_L3_Belady\n\n");
	printf("Section     \tCount     \tPKL2A\n");
	printf("L2_Accesses \t%-10llu\t%-10.3f\n",Time,1000.0);
	printf("L2_Hit      \t%-10llu\t%-10.3f\n",L2_Hit,1000.0*L2_Hit/Time);
	printf("L2_Miss     \t%-10llu\t%-10.3f\n",L2_Miss,1000.0*L2_Miss/Time);
	printf("L3_Hit      \t%-10llu\t%-10.3f\n",L3_Hit,1000.0*L3_Hit/Time);
	printf("L3_Miss     \t%-10llu\t%-10.3f\n-\n",L3_Miss,1000.0*L3_Miss/Time);
    printf("Cold_Miss   \t%-10llu\t\n",Cold_Miss);
    printf("Cap_Miss    \t%-10llu\t\n",Cap_Miss);

	free_cache(L2,L2B,L2R,L2A);
	free_heap(heap3,A3);
	free_hash(hash3,hsz3);
	free_hash2(hashb,hszb);
    free_hash_cold(hashc,hszc);

    return 0;
}


