typedef struct Cold{
	unsigned long long  blkaddr;
	struct Cold * next;
}cold;

int hash_cold_insert(cold ** htbl,unsigned long long Addr,int B,unsigned int hsz);
void free_hash_cold(cold ** htbl, unsigned int hsz);
cold ** hash_init_cold(unsigned int hsz);

