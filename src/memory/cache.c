/* Simulate the (main) behavor of CACHE. Although this will lower the performace of NEMU,
 * it makes you clear about how CACHE is read/written.
 */

#include "common.h"
#include "lib/misc.h"
#include "cpu/reg.h"
#include "stdlib.h"

uint32_t dram_read(hwaddr_t addr, size_t len);
uint32_t dram_write(hwaddr_t addr, size_t len, uint32_t data);


#define BURST_LEN 8
#define BURST_MASK (BURST_LEN - 1)

#define SET_WIDTH 7
#define BLOCK_WIDTH 6
#define TAG_WIDTH 14

typedef union {
	struct {
		uint32_t offset : BLOCK_WIDTH;
		uint32_t set   : SET_WIDTH;
		uint32_t tag   : TAG_WIDTH;
	};
	uint32_t addr;
} L1_addr;

#define NR_BLOCK (1 << BLOCK_WIDTH)
#define NR_SET   (1 << SET_WIDTH)
#define NR_WAY   8
#define BLOCK_MASK (NR_BLOCK - 1)

typedef struct {
	uint8_t blk[NR_BLOCK];
	bool valid;
	uint32_t tag : TAG_WIDTH;
} L1_cache;

L1_cache L1[NR_SET][NR_WAY];

void init_L1() {
	Log("L1 initiing");
	int i, j;
	for (i = 0; i < NR_SET; i ++) {
		for (j = 0; j < NR_WAY; j ++) {
			L1[i][j].valid = false;
		}
	}
}

static void L1_read(swaddr_t addr, void *data) {
	
	L1_addr temp;
	temp.addr = addr & ~BURST_MASK;
	uint32_t set = temp.set;
	uint32_t tag = temp.tag;
	uint32_t offset = temp.offset;

	Log("addr=%x, tag=%x, set=%x, off=%x", temp.addr, tag, set ,offset);
	uint32_t way;
	for (way = 0; way < NR_WAY; way ++) { // search by tag
		if (L1[set][way].tag == tag && L1[set][way].valid) {
			break;
		}
	}
	if (way == NR_WAY) { // miss
		Log("hit");
		// TODO load from L2
		for (way = 0; way < NR_WAY; way ++)
			if (!L1[set][way].valid) // empty block
				break;
		if (way == NR_WAY) // full
			way = rand() % NR_WAY;

		int i;
		hwaddr_t load = addr & ~BLOCK_MASK;
		Log("load=%x", load);
		for (i = 0; i < NR_BLOCK; i ++)
			L1[set][way].blk[i] = dram_read(load + i, 1);
		L1[set][way].valid = true;
		L1[set][way].tag = tag;
	}

	// burst read
	memcpy(data, L1[set][way].blk + offset, BURST_LEN);
}
uint32_t L1_cache_read(swaddr_t addr, size_t len) {
	assert(len == 1 || len == 2 || len == 4);
	uint32_t offset = addr & BURST_MASK;
	uint8_t temp[ 2 * BURST_LEN ];

	L1_read(addr, temp);

	if ( (addr ^ (addr + len - 1)) & ~(BURST_MASK) ) {
		L1_read(addr + BURST_LEN, temp + BURST_LEN);
	}

	return *(uint32_t*)(temp + offset) & (~0u >> ((4 - len) << 3));
}

void L1_write(swaddr_t addr, void *data, uint8_t *mask) {
	L1_addr temp;
	temp.addr = addr & ~BURST_MASK;
	uint32_t set = temp.set;
	uint32_t offset = temp.offset;
	const uint32_t tag = temp.tag;

	uint32_t way;
	for (way = 0; way < NR_WAY; way ++)
		if (L1[set][way].valid && tag == L1[set][way].tag)
			break;

	if (way == NR_WAY) // not write allocate
		return;

	// burst write
	memcpy_with_mask(L1[set][way].blk + offset, data, BURST_LEN, mask);
}

void L1_cache_write(swaddr_t addr, size_t len, uint32_t data) {
	uint32_t offset = addr & BURST_MASK;
	uint8_t temp [2 * BURST_LEN];
	uint8_t mask [2 * BURST_LEN];
	memset(mask, 0, 2 * BURST_LEN);

	*(uint32_t*)(temp + offset) = data;
	memset(mask + offset, 1, len);

	L1_write(addr, temp, mask);

	if ( (addr ^ (addr + len - 1)) & ~(BURST_MASK) ) {
		// data cross the boundary
		L1_write(addr + BURST_LEN, temp + BURST_LEN, mask + BURST_LEN);
	}

	dram_write(addr, len, data); // write through
}

void L1_print(swaddr_t addr) {
	L1_addr temp;
	temp.addr = addr;
	uint32_t set = temp.set;
	const uint32_t tag = temp.tag;
	
	int way;
	for (way = 0; way < NR_WAY; way ++) {
		if (L1[set][way].valid && tag == L1[set][way].tag) {
			printf("set=%x, way=%x:\n", set, way);
			int offset;
			for (offset = 0; offset < NR_BLOCK; offset ++) {
				printf(" %02x", L1[set][way].blk[offset]);
				if (offset == 31) printf("\n");
			}
			printf("\n");
		}
	}
}
