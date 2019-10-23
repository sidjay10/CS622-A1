#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<string.h>
#include"cold.h"

typedef unsigned long long ull;
typedef unsigned int typ;

int hash_cold_insert(cold ** htbl,ull Addr,int B,typ hsz)
{
	ull blkaddr = Addr/B;
	typ idx = blkaddr%hsz;
	cold * ptr = NULL;
	cold * prev = NULL;
	ptr = htbl[idx];

	if(htbl[idx]==NULL)
	{
		cold * new;
		new = (cold *)malloc(sizeof(cold ));
		new->blkaddr = blkaddr;
		new->next = NULL;
		htbl[idx]=new;
		return 1;
	}
	else if(htbl[idx]->blkaddr == blkaddr)
	{
		return 0;
	}
	else
	{
		prev = htbl[idx];
		ptr = htbl[idx]->next;
	}

	while(ptr!=NULL && ptr->blkaddr != blkaddr)
	{
		prev = ptr;
		ptr = ptr->next;
	}
	if(ptr==NULL)
	{

		cold * new;
		new = (cold *)malloc(sizeof(cold ));
		new->blkaddr = blkaddr;
		new->next = NULL;
		prev->next = new;
		return 1;
	}
	else
	{
		return 0;
	}
}

void free_hash_cold(cold ** htbl, typ hsz)
{

	for(typ i=0;i<hsz;i++)
	{
		cold * ptr;
		ptr = htbl[i];
		while(ptr!=NULL)
		{
			cold * ptr1;
			ptr1 = ptr;
			ptr = ptr->next;
			free(ptr1);
		}
	}
	free(htbl);
}

cold ** hash_init_cold(typ hsz)
{
	cold **htbl;
	htbl = (cold **)malloc(hsz*sizeof(cold *));
	memset(htbl,0,hsz*sizeof(cold *));
	return htbl;
}


