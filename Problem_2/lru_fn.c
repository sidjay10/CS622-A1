#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>
#include "lru_fn.h"


int ls(chp * a , chp * b)
{
	if(a->valid == 0 && b->valid)
	{
		return 1;
	}
	else if(a->valid && b->valid == 0)
	{
		return 0;
	}
	else if(a->valid == 0 && b->valid == 0)
	{
		return 0;
	}
	else
	{
		return (a->time < b->time);
	}
}

//invalidate a cache block
typ invalidate(chp ** heap, typ A, chp * blk)
{
	typ idx = blk->idx;
	blk->valid = 0;
	blk->time = 0;
	return heapify_up(heap,idx,A);
}

//insert a cache block after evicting a block
ull evict_insert(chp ** heap, typ A, chp * blk, chs ** hash,typ hsz)
{
	typ idx = 0;
	chp * rm = heap[0];
	heap[0] = blk;
	heap[0]->idx = 0;
	heapify_dn(heap,0,A);

	int v = rm->valid;
	ull blka = rm->blkaddr;

	delete_htbl(hash,rm->blkaddr,hsz);
	free(rm);

	if(v)
		return blka;
	else
		return -1;
}

//cache hit
typ hit(chp ** heap, typ A, chp * blk, ull Time)
{
	typ idx = blk->idx;
	blk->time = Time;
	return heapify_dn(heap,idx,A);	
}

//cache miss
ull miss(chp ** heap, typ A, int B, ull Addr, ull Time, chs ** hash,typ hsz)
{
	chp *new = (chp *)malloc(sizeof(chp));
	new->blkaddr = (Addr/B);
	new->tag = (Addr/B);
	new->time = Time;
	new->valid = 1;

	chs *new_hs = (chs *)malloc(sizeof(chs ));
	new_hs->hp = new;
	new_hs->next = NULL;
	new_hs->blkaddr = (Addr/B);
	new->hs = new_hs;

	insert_htbl(hash,new_hs,hsz);
	return evict_insert(heap,A,new,hash,hsz);
}


typ last_child(typ i,typ A)
{

	if(is_pow2(i+1))
		return A-1;
	else if(i<A)
	{
		while(i<A)
		{
			typ rc = 2*(i+1);
			i = rc;
		}
		return (i-1)/2;
	}
	else
		return -1;
}

typ heapify_up(chp ** heap, typ i, typ A)
{
	if(i==0)
	{
		return i;
	}
	else if(i==A-1)
	{
		if(ls(heap[i],heap[i/2]))
		{
			chp *tmp;
			tmp = heap[i];
			heap[i] = heap[i/2];
			heap[i/2] = tmp;
			heap[i]->idx = i;
			heap[i/2]->idx = i/2;
			return heapify_up(heap,i/2,A);
		}
		else
		{
			return i;
		}
	}
	else if(i<A)
	{
		if(ls(heap[i],heap[(i-1)/2]))
		{
			if(i%2)
			{
				if(ls(heap[i+1],heap[i]))
				{
					chp *tmp;
					tmp = heap[i+1];
					heap[i+1] = heap[(i-1)/2];
					heap[(i-1)/2] = tmp;
					heap[i+1]->idx = i+1;
					heap[(i-1)/2]->idx = (i-1)/2;
					return heapify_up(heap,(i-1)/2,A);
				}
				else
				{
					chp *tmp;
					tmp = heap[i];
					heap[i] = heap[(i-1)/2];
					heap[(i-1)/2] = tmp;
					heap[i]->idx = i;
					heap[(i-1)/2]->idx = (i-1)/2;
					return heapify_up(heap,(i-1)/2,A);
				}
			}
			else
			{
				if(ls(heap[i-1],heap[i]))
				{
					chp *tmp;
					tmp = heap[i-1];
					heap[i-1] = heap[(i-1)/2];
					heap[(i-1)/2] = tmp;
					heap[i-1]->idx = i-1;
					heap[(i-1)/2]->idx = (i-1)/2;
					return heapify_up(heap,(i-1)/2,A);
				}
				else
				{
					chp *tmp;
					tmp = heap[i];
					heap[i] = heap[(i-1)/2];
					heap[(i-1)/2] = tmp;
					heap[i]->idx = i;
					heap[(i-1)/2]->idx = (i-1)/2;
					return heapify_up(heap,(i-1)/2,A);
				}
			}
		}
		else
		{
			return i;
		}
	}
	else
	{
		return -1;
	}
}

typ heapify_dn(chp ** heap, typ i, typ A)
{
	if(i>A)
	{
		return (i-1)/2;
	}

	typ lc = 2*i + 1;
	typ rc = 2*(i+1);

	if(i==(A-1)/2)
	{

		if(ls(heap[lc],heap[i]))
		{
			chp *tmp;
			tmp = heap[lc];
			heap[lc] = heap[i];
			heap[i] = tmp;
			heap[lc]->idx = lc;
			heap[i]->idx = i;
			return lc;
		}
		else
		{
			return i;
		}
	}
	

	if(lc > A || rc > A)
	{
		return i;
	}

	if(ls(heap[rc],heap[i]) && ls(heap[lc], heap[i]))
	{
		if(ls(heap[rc],heap[lc]))
		{
			chp *tmp;
			tmp = heap[rc];
			heap[rc] = heap[i];
			heap[i] = tmp;
			heap[rc]->idx = rc;
			heap[i]->idx = i;
			return heapify_dn(heap,rc,A);
		}
		else
		{
			chp *tmp;
			tmp = heap[lc];
			heap[lc] = heap[i];
			heap[i] = tmp;
			heap[lc]->idx = lc;
			heap[i]->idx = i;
			return heapify_dn(heap,lc,A);
		}
	}
	else if(ls(heap[lc],heap[i]))
	{
		chp *tmp;
		tmp = heap[lc];
		heap[lc] = heap[i];
		heap[i] = tmp;
		heap[lc]->idx = lc;
		heap[i]->idx = i;
		return heapify_dn(heap,lc,A);
	}
	else if(ls(heap[rc],heap[i]))
	{
		chp *tmp;
		tmp = heap[rc];
		heap[rc] = heap[i];
		heap[i] = tmp;
		heap[rc]->idx = rc;
		heap[i]->idx = i;
		return heapify_dn(heap,rc,A);
	}
	else
	{
		return i;
	}
}

int is_pow2(typ x)
{
	return ((x != 0) && ((x & (~x + 1)) == x));
}

int insert_htbl(chs ** htbl, chs * ent,typ sz)
{

	ull blkaddr = ent->blkaddr;
	typ idx = blkaddr % sz;
	chs * ptr;
	ptr = htbl[idx];
	if(htbl[idx]==NULL)
	{
		htbl[idx]= ent;
		return 1;
	}
	while(ptr->next!=NULL)
	{
		ptr = ptr->next;
	}
	ptr->next = ent;
	return 2;
}

int delete_htbl(chs ** htbl, ull blkaddr,typ sz)
{
	//ull blkaddr = ent->blkaddr;
	typ idx = blkaddr % sz;
	chs * ptr;
	ptr = htbl[idx];
	if(htbl[idx]==NULL)
	{
		return 0;
	}
	if(ptr->blkaddr == blkaddr)
	{
		htbl[idx] = ptr->next;
		free(ptr);
		return 1;
	}

	chs* ptr_prev = htbl[idx];
	ptr = ptr->next;

	while(ptr!=NULL && ptr->blkaddr != blkaddr)
	{
		ptr_prev = ptr;
		ptr = ptr->next;
	}
	if(ptr==NULL)
	{
		return 0;
	}
	else
	{
		ptr_prev->next = ptr->next;
		free(ptr);
		return 1;
	}
}

chs * search_htbl(chs ** htbl, ull blkaddr,typ sz)
{
	typ idx = blkaddr % sz;
	chs * ptr = NULL;
	ptr = htbl[idx];
	while(ptr!=NULL && ptr->blkaddr != blkaddr)
	{
		ptr = ptr->next;
	}
	return ptr;
}

chp ** heap_init(typ A)
{
	chp ** heap;
	heap = (chp **)malloc(A*sizeof(chp *));
	memset(heap,0,A*sizeof(chp *));

	for(int i=0;i<A;i++)
	{
		chp * p;
		p = (chp *)malloc(sizeof(chp ));
		memset(p,0,sizeof(chp ));
		p->idx = i;
		heap[i] = p;
	}
	return heap;
}

chs ** hash_init(typ hsz)
{
	chs **htbl;
	htbl = (chs **)malloc(hsz*sizeof(chs *));
	memset(htbl,0,hsz*sizeof(chs *));
	return htbl;
}

void free_heap(chp ** heap, typ A)
{
	for(typ i=0;i<A;i++)
	{
		chp * p;
		p = heap[i];
		free(p);
	}
	free(heap);
}

void free_hash(chs ** htbl, typ hsz)
{

	for(typ i=0;i<hsz;i++)
	{
		chs * ptr;
		ptr = htbl[i];
		while(ptr!=NULL)
		{
			chs * ptr1;
			ptr1 = ptr;
			ptr = ptr->next;
			free(ptr1);
		}
	}
	free(htbl);
}
