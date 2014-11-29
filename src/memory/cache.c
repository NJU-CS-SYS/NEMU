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
	int bit_block;
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

	while (nr_block > 1) {
		nr_block = nr_block >> 1;
		bit_block ++;
	}

	pcache->bit_block = bit_block;
	pcache->mask_set = (nr_set - 1) << bit_block;

	while (nr_set > 1) {
		nr_set = nr_set >> 1;
		bit_set ++;
	}
	Log("bit set = %d", bit_set);
	pcache->mask_tag = (~0u >> (bit_set + bit_block)) << (bit_set + bit_block);
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

void sram_read(swaddr_t raw_addr, void* data) {
	swaddr_t addr = raw_addr & (~BURST_MASK); // aligned addr, if this is not found, the part we want also miss
	uint32_t tag = addr & head->mask_tag;
	uint32_t set = (addr & head->mask_set) >> head->bit_block; // note that set is a numeric data
	uint32_t offset = addr & head->mask_block; // used to locate data in block

	// search the cached data
	int way;
	for (way = 0; way < head->nr_way; way ++) {
		if (head->cache[set][way].valid && head->cache[set][way].tag == tag) {
			break;
		}
	}

	// if miss
	if (way == head->nr_way) {
		// find a empty block in this set
		for (way = 0; way < head->nr_way; way++) {
			if (!head->cache[set][way].valid) {
				break;
			}
		}
		// if full, just take the first one
		// TODO use fake lru algo !
		if (way == head->nr_way) {
			way = 0;
		}
		// load from dram
		head->cache[set][way].valid = 1;
		head->cache[set][way].tag = tag;
		swaddr_t load_addr = tag | (set << head->bit_block);;
		Log("the addr to be loaded is %x", load_addr);
		int i;
		for (i = 0; i < head->nr_block; i ++) {
			head->cache[set][way].block[i] = dram_read(load_addr + i, 1);
		}
	}
	memcpy(data, head->cache[set][way].block + offset, BURST_LEN);
}

// this function handle the situation
// when the data is cross the boundary
uint32_t cache_read(swaddr_t addr, size_t len) {
	Log("input addr is %x", addr);
	assert(len == 1 || len == 2 || len == 4);
	uint32_t offset = addr & BURST_MASK;
	uint8_t temp[2 * BURST_LEN];

	sram_read(addr, temp);

	if ( (addr ^ (addr + len - 1)) & ~(BURST_MASK) ) {
		// data cross the burst boundary
		Log("override addr will be %x", addr + BURST_LEN);
		sram_read(addr + BURST_LEN, temp + BURST_LEN);
	}
	return *(uint32_t*)(temp + offset) & (~0u >> ((4 - len) << 3));
}

// print the cache, especially for head(L1 cache)
void print_cache(swaddr_t addr) {
	uint32_t set = addr & head->mask_set;
	set = set >> head->bit_block;
	int way, blck;
	printf("set %d\n", set);
	for (way = 0; way < head->nr_way; way ++) {
		printf("%d : tag = %x", way, head->cache[set][way].tag);
		for (blck = 0; blck < head->nr_block; blck ++) {
			printf(" %02x", head->cache[set][way].block[blck]);
			if (blck == 31) printf("\n");
		}
		printf("\n");
	}
}
