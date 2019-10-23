#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>

#include"belady_fn.h"

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
		return (a->time > b->time);
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
	ull Time = blk->time;
	typ idx = 0;

	chp * rm = heap[0];

	chp * last = heap[A-1];
	heap[0] = heap[A-1];
	heap[0] ->idx = 0;

	heapify_dn_pop(heap,0,A-1);
	
	
	blk->idx = A-1;
	heap[A-1] = blk;
	blk->valid = 1;
	blk->time = Time;

	heapify_up(heap,A-1,A);

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
	blk->valid = 1;
	blk->time = Time;
	return heapify_up(heap,idx,A);
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
	{
		return A-1;
	}
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
	{
		return -1;
	}
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

typ heapify_dn_pop(chp ** heap, typ i, typ A)
{
	if(i>A)
	{
		return (i-1)/2;
	}

	typ lc = 2*i + 1;
	typ rc = 2*(i+1);


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
			return heapify_dn_pop(heap,rc,A);
		}
		else
		{
			chp *tmp;
			tmp = heap[lc];
			heap[lc] = heap[i];
			heap[i] = tmp;
			heap[lc]->idx = lc;
			heap[i]->idx = i;
			return heapify_dn_pop(heap,lc,A);
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
		return heapify_dn_pop(heap,lc,A);
	}
	else if(ls(heap[rc],heap[i]))
	{
		chp *tmp;
		tmp = heap[rc];
		heap[rc] = heap[i];
		heap[i] = tmp;
		heap[rc]->idx = rc;
		heap[i]->idx = i;
		return heapify_dn_pop(heap,rc,A);
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
	//printf("%u\n",idx);
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
	//printf("%u\n",idx);
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
		p->valid = 0;
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

bel ** hash_init_bel(typ hsz)
{
	bel **htbl;
	htbl = (bel **)malloc(hsz*sizeof(bel *));
	memset(htbl,0,hsz*sizeof(bel *));
	return htbl;
}

int hash_bel_insert(bel ** htbl,ull Addr, ull Time,int B,typ hsz)
{
	ull blkaddr = Addr/B;
	typ idx = blkaddr%hsz;
	bel * ptr = NULL;
	bel * prev = NULL;
	ptr = htbl[idx];

	if(htbl[idx]==NULL)
	{
		bel * new;
		new = (bel *)malloc(sizeof(bel ));
		new->blkaddr = blkaddr;
		new->next = NULL;

		node * new_node;
		new_node = (node*)malloc(sizeof(node));
		new_node -> time = Time;
		new_node -> next = NULL;
		new->head = new_node;
		new->tail = new_node;
		htbl[idx] = new;
		return 1;
	}
	else if(htbl[idx]->blkaddr == blkaddr)
	{
		node * new_node;
		new_node = (node*)malloc(sizeof(node));
		new_node -> time = Time;
		new_node -> next = NULL;
		htbl[idx]->tail->next = new_node;
		htbl[idx]->tail = new_node;
		return 4;
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

		bel * new;
		new = (bel *)malloc(sizeof(bel ));
		new->blkaddr = blkaddr;
		new->next = NULL;
		prev->next = new;

		node * new_node;
		new_node = (node*)malloc(sizeof(node));
		new_node -> time = Time;
		new_node -> next = NULL;
		new->head = new_node;
		new->tail = new_node;
		return 2;
	}
	else
	{
		node * ptr_node;
		ptr_node = ptr->tail;
		
		node * new_node;
		new_node = (node*)malloc(sizeof(node));
		new_node -> time = Time;
		new_node -> next = NULL;
		ptr_node->next = new_node;
		ptr->tail = new_node;
		return 3;

	}
}

ull hash_bel_search(bel ** htbl,ull Addr,int B,typ hsz)
{
	ull blkaddr=Addr/B;
	typ idx = blkaddr%hsz;
	bel * ptr = NULL;
	ptr = htbl[idx];

	ull T = -1;

	while(ptr!=NULL && ptr->blkaddr != blkaddr)
	{
		ptr = ptr->next;
	}
	if(ptr == NULL)
	{
		return T;
	}
	else if(ptr->head == NULL)
	{
		return T;
	}
	else
	{
		return ptr->head->time;
	}
}

int hash_bel_update(bel ** htbl,ull Addr,int B,typ hsz)
{
	ull blkaddr=Addr/B;
	typ idx = blkaddr%hsz;
	bel * ptr = NULL;
	ptr = htbl[idx];

	while(ptr!=NULL && ptr->blkaddr != blkaddr)
	{
		ptr = ptr->next;
	}
	if(ptr == NULL)
	{
		return -1;
	}
	else if(ptr->head == NULL)
	{
		return 0;
	}
	else
	{
		node * del;
		del = ptr->head;
		ptr->head = del->next;
		free(del);
		return 1;
	}
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

void free_hash2(bel ** htbl, typ hsz)
{

	for(typ i=0;i<hsz;i++)
	{
		bel * ptr;
		ptr = htbl[i];
		while(ptr!=NULL)
		{
			node * ptrn;
			ptrn = ptr->head;
			while(ptrn!=NULL)
			{
				node* ptrn1;
				ptrn1 = ptrn;
				ptrn = ptrn->next;
				free(ptrn1);
			}
			bel * ptr1;
			ptr1 = ptr;
			ptr = ptr->next;
			free(ptr1);
		}
	}
	free(htbl);
}

