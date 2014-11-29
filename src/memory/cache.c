#include "common.h"
#include "lib/misc.h"
#include "cpu/reg.h"
#include "stdlib.h"

/* Simulate the (main) behavor of CACHE. Although this will lower the performace of NEMU,
 * it makes you clear about how CACHE is read/written.
 */

typedef struct {
	uint8_t *block;
	uint8_t valid;
	uint8_t used;
} block;

struct _cache_ {
	block **cache;
	int nr_set;
	int nr_way;
	int nr_block;
	int mask_set;
	int mask_block;
	struct _cache_ *next;
};
typedef struct _cache_ cache;

static cache* head;

bool init_cache (cache *pcache) {
	test(pcache != NULL, "the cache isn's allocated!");
	
	int i, j, k;
	for (i = 0; i < pcache->nr_set; i ++) {
		for (j = 0; j < pcache->nr_way; j ++) {
			pcache->cache[i][j].valid = 0;
			for (k = 0; k < pcache->nr_block; k ++) {
				pcache->cache[i][j].block[k] = 0;
			}
		}
	}

	return true;
}

bool create_cache(cache **ppcache, int x, int y, int z) {
	(*ppcache) = (cache*)malloc(sizeof(cache));
	cache* pcache = *ppcache;
	pcache->nr_set = x;
	pcache->nr_way = y;
	pcache->nr_block = z;
	pcache->next = NULL;

	// allocate the mem for data
	int i, j;
	pcache->cache = (block**)malloc(sizeof(block*) * x);
	for (i = 0; i < x; i ++) {
		pcache->cache[i] = (block*)malloc(sizeof(block) * y);
		for (j = 0; j < y; j ++) {
			pcache->cache[i][j].block = (uint8_t*)malloc(sizeof(uint8_t) * z);
		}
	}

	// init
	init_cache(pcache);
	return true;
}

void delete_cache() {
	while (head != NULL) {
		cache *ptemp = head;
		head = head->next;

		int i, j;
		for (i = 0; i < ptemp->nr_set; i ++) {
			for (j = 0; j < ptemp->nr_way; j ++) {
				free(ptemp->cache[i][j].block);
			}
			free(ptemp->cache[i]);
		}
		free(ptemp->cache);
		free(ptemp);
	}
}

#if 0
bool hit_cache(cache *pcache, swaddr_t addr, void* data) {
	
	int bit_set, bit_block;
	int nr_set, nr_way, nr_bloc;
	nr_set = pcache->nr_set;
	nr_way = pcache->nr_way;
	nr_block = pcache->nr_block;
#endif
