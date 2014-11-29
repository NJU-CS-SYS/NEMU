/* Simulate the (main) behavor of CACHE. Although this will lower the performace of NEMU,
 * it makes you clear about how CACHE is read/written.
 */

#include "common.h"
#include "lib/misc.h"
#include "cpu/reg.h"
#include "stdlib.h"

#define BURST_LEN 8
#define BURST_MASK (BURST_LEN - 1)

typedef struct {
	uint32_t tag;
	uint8_t *block;
	uint8_t valid;
	uint8_t dirty;
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
	uint8_t en_wrt_alloc;
	uint8_t en_wrt_back;
	struct _cache_ *next;
};
typedef struct _cache_ cache;

uint32_t dram_read(hwaddr_t addr, size_t len);
uint32_t dram_write(hwaddr_t addr, size_t len, uint32_t data);


void init_nemu_cache();
bool init_cache();
cache* create_cache(uint32_t,uint32_t,uint32_t,uint8_t,uint8_t);
void in_cache_write(swaddr_t,size_t,uint32_t,cache*);
uint32_t in_cache_read(swaddr_t,size_t,cache*);
void delete_cache();

static cache *head;

void init_nemu_cache() {
	head = create_cache(128, 8, 64, 0, 0);
	//head->next = create_cache(4096, 16, 64, 1, 1);
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
	pcache->mask_tag = (~0u >> (bit_set + bit_block)) << (bit_set + bit_block);
	// data init
	int i, j;
	for (i = 0; i < pcache->nr_set; i ++) {
		for (j = 0; j < pcache->nr_way; j ++) {
			pcache->cache[i][j].valid = 0;
			pcache->cache[i][j].dirty = 1;
			memset(pcache->cache[i][j].block, 0, pcache->nr_block);
		}
	}

	return true;
}

cache* create_cache(uint32_t x, uint32_t y, uint32_t z, uint8_t en_wrt_alloc, uint8_t en_wrt_back) {
	cache *pcache = (cache*)malloc(sizeof(cache));
	pcache->nr_set = x;
	pcache->nr_way = y;
	pcache->nr_block = z;
	pcache->en_wrt_alloc = en_wrt_alloc;
	pcache->en_wrt_back = en_wrt_back;
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

static uint32_t cache_miss_alloc(uint32_t tag, uint32_t set, cache *p) { // allocate when miss cache
	int way;
	block** cache = p->cache;
	swaddr_t addr = tag | (set << p->bit_block);

	// find an empty block
	for (way = 0; way < p->nr_way; way++)
		if (!p->cache[set][way].valid) break;
	
	if (way == p->nr_way) { // replacement, using rand algorithm
		srand(addr);
		way = rand() % p->nr_way;

	assert(0);
		if (p->en_wrt_back && cache[set][way].dirty) { // write back
			hwaddr_t back_addr = tag | (set << p->bit_block);
			uint8_t* blk = cache[set][way].block;

			int i;
#if 0
			if (p->next == NULL) { // access physical memory
				for (i = 0; i < p->nr_block; i ++)
					dram_write(back_addr + i, 1, blk[i]);
			} else { // next cache level
				for (i = 0; i < p->nr_block; i ++)
					in_cache_write(back_addr + i, 1, blk[i], p->next);
			}
#endif
#if 1
			for (i = 0; i < p->nr_block; i ++)
			in_cache_write(back_addr + i, 1, blk[i], p->next);
#endif
		}
	}

	// load from dram
	int i;
	p->cache[set][way].valid = 1;
	p->cache[set][way].tag = tag;
	uint8_t *blk = p->cache[set][way].block;
#if 0
	if (p->next == NULL) { // access physical memory
		for (i = 0; i < p->nr_block; i ++)
			blk[i] = dram_read(addr + i, 1);
	} 
	else { // next cache level
		for (i = 0; i < p->nr_block; i ++)
			blk[i] = in_cache_read(addr + i, 1, p->next);
	}
#endif
#if 1
	for (i = 0; i < p->nr_block; i ++)
		blk[i] = in_cache_read(addr + i, 1, p->next);
#endif

	return way;
}

void sram_read(swaddr_t raw_addr, void* data, cache *p) {
	block** cache = p->cache;
	swaddr_t addr = raw_addr & (~BURST_MASK); // aligned addr, if this is not found, the part we want also miss
	uint32_t tag = addr & p->mask_tag;
	uint32_t set = (addr & p->mask_set) >> p->bit_block; // note that set is a numeric data
	uint32_t offset = addr & p->mask_block; // used to locate data in block
	int way;

	// search
	for (way = 0; way < p->nr_way; way ++)
		if (cache[set][way].valid && cache[set][way].tag == tag)
			break;

	// miss
	if (way == p->nr_way) 
		way = cache_miss_alloc(tag, set, p);

	memcpy(data, cache[set][way].block + offset, BURST_LEN);
}

void sram_write(swaddr_t raw_addr, void *data, uint8_t *mask, cache *p) {
	block** cache = p->cache;
	swaddr_t addr = raw_addr & (~BURST_MASK);
	uint32_t tag = addr & p->mask_tag;
	uint32_t set = (addr & p->mask_set) >> p->bit_block;
	uint32_t offset = addr & p->mask_block;

	// search
	int way;
	for (way = 0; way < p->nr_way; way ++)
		if (cache[set][way].valid && cache[set][way].tag == tag)
			break;

	// miss
	if (way == p->nr_way) {
		if (p->en_wrt_alloc) way = cache_miss_alloc(tag, set, p);
	} else {
		cache[set][way].dirty = 1;
	}

	// burst write
	memcpy_with_mask(p->cache[set][way].block + offset, data, BURST_LEN, mask);
}

uint32_t in_cache_read(swaddr_t addr, size_t len, cache *p) {
	assert(len == 1 || len == 2 || len == 4);
	uint32_t offset = addr & BURST_MASK;
	uint8_t temp[2 * BURST_LEN];

	sram_read(addr, temp, p);

	if ( (addr ^ (addr + len - 1)) & ~(BURST_MASK) ) {
		// data cross the burst boundary
		sram_read(addr + BURST_LEN, temp + BURST_LEN, p);
	}
	return *(uint32_t*)(temp + offset) & (~0u >> ((4 - len) << 3));
}

void in_cache_write(swaddr_t addr, size_t len, uint32_t data, cache *p) {
	uint32_t offset = addr & BURST_MASK;
	uint8_t temp[2 * BURST_LEN];
	uint8_t mask[2 * BURST_LEN];
	memset(mask, 0, 2 * BURST_LEN);

	*(uint32_t*)(temp + offset) = data;
	memset(mask + offset, 1, len);

	sram_write(addr, temp, mask, p);

	if ( (addr ^ (addr + len - 1)) & (~BURST_MASK) ) {
		// data cross the boundary
		sram_write(addr, temp, mask, p);
	}

	// write through
	if(!p->en_wrt_back) dram_write(addr, len, data);
}

uint32_t cache_read(swaddr_t addr, size_t len) {
	return in_cache_read(addr, len, head);
}

void cache_write(swaddr_t addr, size_t len, uint32_t data) {
	in_cache_write(addr, len, data, head);
}

void print_cache(swaddr_t addr) {
	block** cache = head->cache;
	uint32_t set = addr & head->mask_set;
	uint32_t tag = addr & head->mask_tag;
	set = set >> head->bit_block;
	int way, blck;
	printf("set %d\n", set);
	for (way = 0; way < head->nr_way; way ++) {
		if (tag == cache[set][way].tag && cache[set][way].valid) {
			printf("way %x: tag = %x\n", way, cache[set][way].tag);
			for (blck = 0; blck < head->nr_block; blck ++) {
				printf(" %02x", cache[set][way].block[blck]);
				if (blck == 31) printf("\n");
			}
			printf("\n");
		}
	}
}
