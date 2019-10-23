/*  file that contains definitions of all function used to model a
    set associative LRU cache   */

#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<string.h>
#include"cache_fn.h"

/*	Function to simulate Inclusive Policy */
void Inclusive(char** argv)
{
	cache **L2;
	cache **L3; 
	L2 = init(L2B,L2R,L2A);
	L3 = init(L3B,L3R,L3A);

	char output_name[100];
	unsigned long long Addr;
	
	ull Time = 0;
	ull L2_Miss = 0;
	ull L2_Hit = 0;
	ull L3_Miss = 0;
	ull L3_Hit = 0;

	ull Incl_Victim = 0;

	FILE* fp;
	sprintf(output_name, "%s_output", argv[1]);
	fp = fopen(output_name, "rb");
	assert(fp != NULL);


	while (!feof(fp)) {
		fread(&Addr, sizeof(unsigned long long), 1, fp);
		Time++;
		if(Hit_Miss(L2,L2R,L2A,L2B,Addr,Time))
		{
			L2_Hit++;
		}
		else
		{
			L2_Miss++;
			if(Hit_Miss(L3,L3R,L3A,L3B,Addr,Time))
			{
				ull Add_Evi_L2 = Miss_Handler(L2,L2B,L2R,L2A,Addr,Time);
				L3_Hit++;

			}
			else
			{
				L3_Miss++;
				ull Add_Evi_L3 = Miss_Handler(L3,L3B,L3R,L3A,Addr,Time);
				if(Add_Evi_L3 != -1)
				{
					;
					Incl_Victim += Invalidate(L2,L2B,L2R,L2A,Add_Evi_L3);
				}
				ull Add_Evi_L2 = Miss_Handler(L2,L2B,L2R,L2A,Addr,Time);
			}
		}
	}
	fclose(fp);

    printf("Section     \tCount     \tPKL2A\n");
	printf("L2_Accesses \t%-10llu\t%-10.3f\n",Time,1000.0);
	printf("L2_Hit      \t%-10llu\t%-10.3f\n",L2_Hit,1000.0*L2_Hit/Time);
	printf("L2_Miss     \t%-10llu\t%-10.3f\n",L2_Miss,1000.0*L2_Miss/Time);
	printf("L3_Hit      \t%-10llu\t%-10.3f\n",L3_Hit,1000.0*L3_Hit/Time);
	printf("L3_Miss     \t%-10llu\t%-10.3f\n-\n",L3_Miss,1000.0*L3_Miss/Time);
	
	free_cache(L2,L2B,L2R,L2A);
	free_cache(L3,L3B,L3R,L3A);
    
}

//function to free memory occupied by caches
void free_cache(cache** C, int B, int R, int A)
{
	for(int i=0;i<R;i++)
	{
		free(C[i]);
	}
	free(C);
}

//function to initialise caches
cache ** init(int B,int R,int A)
{
	cache ** C;
	C = (cache **)malloc(R*sizeof(cache *));
	for(int i=0;i<R;i++)
	{
		C[i] = (cache *)malloc(A*sizeof(cache ));
		memset(C[i],0,A*sizeof(cache));
	}
	return C;
}

/*	function to invalidate the cache block corresponding
	to an address (word address) in a cache
	it returns 1 if the block was valid and is invalidated;
	returns 0 if the block was not present or already invalid */
int Invalidate(cache ** C,int B,int R,int A,ull Addr)
{
	int Idx = (Addr / (B))%(R);
	ull Tag = (Addr / (B*R));

	for(int i=0;i<A;i++)
	{
		if(C[Idx][i].valid && C[Idx][i].tag == Tag)
		{
			C[Idx][i].valid = 0;
			return 1;
		}
	}	
	return 0;
}

/*	Function to handle a miss. It fills up the cache block
	with the block corresponding to the address and if it evicts a valid
	block, returns its address; otherwise returns -1 */ 
ull Miss_Handler(cache ** C,int B,int R,int A,ull Addr, ull Time)
{
	int Idx = (Addr / (B))%(R);
	ull Tag = (Addr / (B*R));
	ull Add_Evi;

	int R_Way = LRU_Block(C[Idx],A);
	ull Tag_Evi;
	if(C[Idx][R_Way].valid)
	{
		Tag_Evi = C[Idx][R_Way].tag;
		Add_Evi = (Tag_Evi*R + Idx)*B;
	}
	else
	{
		Add_Evi = -1;
	}

	C[Idx][R_Way].tag = Tag;
	C[Idx][R_Way].valid = 1;
	C[Idx][R_Way].time = Time;
	
	return Add_Evi;
}

/*	Function to check whether a given block hits in a cache or not
	updates time and returns 1 if it is a hit
	otherwise returns 0 */
int Hit_Miss(cache **C,int R, int A, int B, ull Addr, ull Time)
{
	int Idx = (Addr / (B))%(R);	//Block Index
	ull Tag = (Addr / (B*R));	//Block Tag

	for(int i=0;i<A;i++)
	{
		if(C[Idx][i].valid && C[Idx][i].tag == Tag)
		{
			C[Idx][i].time = Time;
			return 1;
		}
	}	
	return 0;
}

/*	Returns the way number of the LRU block in a given set 
	Returns the way number of an invalid block in the set, if any */
int LRU_Block(cache * Set, int A){
	int min = 0;
	for(int i=0;i<A;i++){
		if(Set[i].valid == 0)
		{
			min = i;
			break;
		}
		else if(Set[i].time < Set[min].time)
		{
			min = i;
		}
	}
	return min;
}
