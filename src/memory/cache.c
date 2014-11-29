#include "common.h"
#include "lib/misc.h"
#include "cpu/reg.h"
#include "stdlib.h"

uint32_t dram_read(hwaddr_t addr, size_t len);
uint32_t dram_write(hwaddr_t addr, size_t len, uint32_t data);
/* Simulate the (main) behavor of CACHE. Although this will lower the performace of NEMU,
 * it makes you clear about how CACHE is read/written.
 */

#define BURST_LEN 8
#define BURST_MASK (BURST_LEN - 1)
typedef struct {
	uint32_t tag;
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
	int mask_tag;
	struct _cache_ *next;
};

typedef struct _cache_ cache;

static cache * head;

void init_nemu_cache();
bool init_cache();
cache* create_cache();
void delete_cache();

void init_nemu_cache() {
	head = create_cache(128, 8, 64);
}

bool init_cache (cache *pcache) {
	test(pcache != NULL, "the cache isn's allocated!");
	
	// get mask
	int nr_block = pcache->nr_block;
	int nr_set = pcache->nr_set;
	int bit_block = 0;
	int bit_set = 0;

	pcache->mask_block = (nr_block - 1);
	Log("mask_block = %x", pcache->mask_block);

	while (nr_block > 1) {
		nr_block = nr_block >> 1;
		bit_block ++;
	}
	Log("nr_block = %d, bit_block = %x", pcache->nr_block, bit_block);

	pcache->mask_set = (nr_set - 1) << bit_block;
	Log("mask_set = %x", pcache->mask_set);	

	while (nr_set > 1) {
		nr_set = nr_set >> 1;
		bit_set ++;
	}
	Log("nr_set = %d, bit_set = %x", pcache->nr_set, bit_set);

	pcache->mask_tag = (~0u >> (bit_set + bit_block)) << (bit_set + bit_block);
	Log("mask_tag = %x", pcache->mask_tag);

	// data init
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

cache* create_cache(int x, int y, int z) {
	cache *pcache = (cache*)malloc(sizeof(cache));
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
			(pcache->cache[i][j]).block = (uint8_t*)malloc(sizeof(uint8_t) * z);
		}
	}

	// init
	init_cache(pcache);
	return pcache;
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

uint32_t read_cache(swaddr_t addr, size_t len) {
	uint32_t tag = addr & head->mask_tag;
	uint32_t set = addr & head->mask_set;
	uint32_t offset = addr & head->mask_block;
	Log("tag = %x, set = %x, offset = %x, addr = %x", tag, set, offset, addr);

	// search the cached data
	int way;
	for (way = 0; way < head->nr_way; way ++) {
		Log("cached tag : %x, cmp tag : %x, valid : %x", head->cache[set][way].tag, tag, head->cache[set][way].valid);
		if (head->cache[set][way].valid && head->cache[set][way].tag == tag) break;
	}
	// miss
	if (way == head->nr_way) {
		Log("miss");
		// find a empty block in this set
		for (way = 0; way < head->nr_way; way++)
			if (!head->cache[set][way].valid)
				break;
		// if full, just take the first one
		if (way == head->nr_way)
			way = 0;
		// load from dram
		head->cache[set][way].valid = 1;
		head->cache[set][way].tag = tag;
		swaddr_t load_addr = tag | set;
		int idx;
		for (idx = 0; idx < head->nr_block; idx ++) {
			head->cache[set][way].block[idx] = dram_read(load_addr + idx, 1);
			//Log("%x ", head->cache[set][way].block[idx]);
		}
	} else {
		Log("hit");
	}

	// buf
	uint8_t temp[ BURST_LEN ];
	memcpy(temp, head->cache[set][way].block + offset, BURST_LEN);
	return *(uint32_t*)temp & (~0u >> ((4 - len) << 3));
}
