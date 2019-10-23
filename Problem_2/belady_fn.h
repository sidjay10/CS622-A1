struct cb_hp;
struct cb_hs;


typedef unsigned long long ull;
typedef unsigned int typ;
typedef struct cb_hp{
	ull blkaddr;
	ull tag;
	ull time;
	char valid;
	typ idx;
	struct cb_hs * hs;
}chp;

typedef struct cb_hs{
	ull blkaddr;
	struct cb_hp * hp;
	struct cb_hs * next;
}chs;


typedef struct Node{
	ull time;
	struct Node * next;
}node;

typedef struct Belady{
	ull blkaddr;
	struct Node* head;
	struct Node* tail;
	struct Belady * next;
}bel;



int ls(chp *, chp*);
int is_pow2(typ x);
typ heapify_up(chp ** heap, typ i, typ A);
typ heapify_dn(chp ** heap, typ i, typ A);
typ last_child(typ i,typ A);
typ hit(chp ** heap, typ A, chp * blk, ull Time);
typ invalidate(chp ** heap, typ A, chp * blk);
int insert_htbl(chs ** htbl, chs * ent,typ sz);
int delete_htbl(chs ** htbl, ull blkaddr,typ sz);
chs * search_htbl(chs ** htbl, ull blkaddr,typ sz);
ull evict_insert(chp ** heap, typ A, chp * blk,chs ** hash,typ hsz);
chs ** hash_init(typ hsz);
chp ** heap_init(typ A);
ull miss(chp ** heap, typ A, int B, ull Addr, ull Time, chs ** hash,typ hsz);
int hash_bel_insert(bel ** htbl,ull Addr, ull Time,int B,typ hsz);
bel ** hash_init_bel(typ hsz);
int hash_bel_update(bel ** htbl,ull Addr,int B,typ hsz);
ull hash_bel_search(bel ** htbl,ull Addr,int B,typ hsz);
void free_hash(chs ** htbl, typ hsz);
void free_heap(chp ** heap, typ A);
void free_hash2(bel ** htbl, typ hsz);
typ heapify_dn_pop(chp ** heap, typ i, typ A);
