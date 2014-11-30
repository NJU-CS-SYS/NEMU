/* Simulate the (main) behavor of CACHE. Although this will lower the performace of NEMU,
 * it makes you clear about how CACHE is read/written.
 */

#include "common.h"
#include "lib/misc.h"
#include "cpu/reg.h"
#include "stdlib.h"

uint32_t dram_read(hwaddr_t addr, size_t len);
uint32_t dram_write(hwaddr_t addr, size_t len, uint32_t data);

void L2_print(swaddr_t addr);

#define BURST_LEN 8
#define BURST_MASK (BURST_LEN - 1)

#define SET_WIDTH 12
#define BLOCK_WIDTH 6
#define TAG_WIDTH 9

typedef union {
	struct {
		uint32_t offset : BLOCK_WIDTH;
		uint32_t set   : SET_WIDTH;
		uint32_t tag   : TAG_WIDTH;
	};
	uint32_t addr;
} L2_addr;

#define NR_BLOCK (1 << BLOCK_WIDTH)
#define NR_SET   (1 << SET_WIDTH)
#define NR_WAY   8
#define BLOCK_MASK (NR_BLOCK - 1)

typedef struct {
	uint8_t blk[NR_BLOCK];
	bool valid;
	uint32_t tag : TAG_WIDTH;
	uint8_t dirty;
} L2_cache;

L2_cache L2[NR_SET][NR_WAY];

void init_L2() {
	int i, j;
	for (i = 0; i < NR_SET; i ++) {
		for (j = 0; j < NR_WAY; j ++) {
			L2[i][j].valid = false;
			L2[i][j].dirty = false;
		}
	}
}

static void L2_read(swaddr_t addr, void *data) {
	Log("Into L2");
	
	L2_addr temp;
	temp.addr = addr & ~BURST_MASK;
	uint32_t set = temp.set;
	uint32_t tag = temp.tag;
	uint32_t offset = temp.offset;

	uint32_t way;
	for (way = 0; way < NR_WAY; way ++) { // search by tag
		if (L2[set][way].tag == tag && L2[set][way].valid) {
			break;
		}
	}

	if (way == NR_WAY) { // miss, allocate
		Log("miss");
		for (way = 0; way <	NR_WAY; way ++) // find empty block
			if (!L2[set][way].valid)
				break;
		if (way == NR_WAY) { // cache full, replacement
			way = rand() % NR_WAY; // random replacement
			if (L2[set][way].dirty) { // write back
				int i;
				hwaddr_t back_addr = (L2[set][way].tag << (SET_WIDTH + BLOCK_WIDTH)) | (set << BLOCK_WIDTH);
				for (i = 0; i < NR_BLOCK; i ++)
					dram_write(back_addr + i, 1, L2[set][way].blk[i]);
				for (i = 0; i < NR_BLOCK; i ++)
					test(dram_read(back_addr + i, 1) == L2[set][way].blk[i], "write back wrong");
			} 
		}	
	}
		// write allocate
	hwaddr_t load = addr & ~BLOCK_MASK;
	int i;
	for (i = 0; i < NR_BLOCK; i ++)
		L2[set][way].blk[i] = dram_read(load + i, 1);

	L2[set][way].tag = tag;

	// burst read
	memcpy(data, L2[set][way].blk + offset, BURST_LEN);
}
uint32_t L2_cache_read(swaddr_t addr, size_t len) {
	assert(len == 1 || len == 2 || len == 4);
	uint32_t offset = addr & BURST_MASK;
	uint8_t temp[ 2 * BURST_LEN ];

	L2_read(addr, temp);

	if ( (addr ^ (addr + len - 1)) & ~(BURST_MASK) ) {
		L2_read(addr + BURST_LEN, temp + BURST_LEN);
	}
	return *(uint32_t*)(temp + offset) & (~0u >> ((4 - len) << 3));
}

void L2_write(swaddr_t addr, void *data, uint8_t *mask) {
	Log("Into L2 write");
	L2_addr temp;
	temp.addr = addr & ~BURST_MASK;
	uint32_t set = temp.set;
	uint32_t offset = temp.offset;
	const uint32_t tag = temp.tag;

	uint32_t way;
	for (way = 0; way < NR_WAY; way ++)
		if (L2[set][way].valid && tag == L2[set][way].tag)
			break;

	if (way == NR_WAY) { // miss, write allocate
		Log("write miss");
		for (way = 0; way <	NR_WAY; way ++) // find empty block
			if (!L2[set][way].valid)
				break;
		if (way == NR_WAY) { // cache full, replacement
			way = rand() % NR_WAY; // random replacement
			if (L2[set][way].dirty) { // write back
				int i;
				hwaddr_t back_addr = (L2[set][way].tag << (SET_WIDTH + BLOCK_WIDTH)) | (set << BLOCK_WIDTH);
				for (i = 0; i < NR_BLOCK; i ++)
					dram_write(back_addr + i, 1, L2[set][way].blk[i]);
				for (i = 0; i < NR_BLOCK; i ++)
					test(dram_read(back_addr + i, 1) == L2[set][way].blk[i], "write back wrong");
			} 
		}	
		// write allocate
		hwaddr_t load = addr & ~BLOCK_MASK;
		int i;
		for (i = 0; i < NR_BLOCK; i ++)
			L2[set][way].blk[i] = dram_read(load + i, 1);

	}
	// burst write
	L2_print(addr);
	memcpy_with_mask(L2[set][way].blk + offset, data, BURST_LEN, mask);
	L2[set][way].dirty = true;
}

void L2_cache_write(swaddr_t addr, size_t len, uint32_t data) {
	Log("data to write: %x", data);
	uint32_t offset = addr & BURST_MASK;
	uint8_t temp [2 * BURST_LEN];
	uint8_t mask [2 * BURST_LEN];
	memset(mask, 0, 2 * BURST_LEN);

	*(uint32_t*)(temp + offset) = data;
	memset(mask + offset, 1, len);

	L2_write(addr, temp, mask);

	if ( (addr ^ (addr + len - 1)) & ~(BURST_MASK) ) {
		// data cross the boundary
		L2_write(addr + BURST_LEN, temp + BURST_LEN, mask + BURST_LEN);
	}

	//dram_write(addr, len, data); // write through
}

void L2_print(swaddr_t addr) {
	L2_addr temp;
	temp.addr = addr;
	uint32_t set = temp.set;
	const uint32_t tag = temp.tag;
	
	int way;
	for (way = 0; way < NR_WAY; way ++) {
		if (L2[set][way].valid && tag == L2[set][way].tag) {
			printf("set=%x, way=%x:\n", set, way);
			int offset;
			for (offset = 0; offset < NR_BLOCK; offset ++) {
				printf(" %02x", L2[set][way].blk[offset]);
				if (offset == 31) printf("\n");
			}
			printf("\n");
		}
	}
}
