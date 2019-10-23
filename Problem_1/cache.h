/*  header file for set associative cache   */

#define L2R 1024
#define L2A 8
#define L2B 64


#define L3R 2048
#define L3A 16
#define L3B 64

typedef unsigned long long ull;

typedef struct Cache_Block{
        char valid;
        unsigned long long tag;
        unsigned long long time;

}cache;

void Exclusive(char ** name);
void NINE(char ** name);
void Inclusive(char ** name);
void free_cache(cache** C, int B, int R, int A);
int Hit_Miss(cache **C,int R, int A, int B, ull Addr, ull Time);
ull Miss_Handler(cache ** C,int B,int R,int A,ull Addr, ull Time);
int Invalidate(cache ** C,int B,int R,int A,ull Addr);
cache ** init(int B,int R,int A);
int LRU_Block(cache * set, int A);
int Cache(cache **,int S, int A, int B, ull Addr, ull Time);
