#include "common.h"

uint32_t L1_cache_read(swaddr_t addr, size_t len);
uint32_t L1_cache_write(swaddr_t addr, size_t len, uint32_t data);
uint32_t L1_print(swaddr_t addr);
uint32_t dram_read(hwaddr_t addr, size_t len);
void dram_write(hwaddr_t addr, size_t len, uint32_t data);

/* Memory accessing interfaces */

uint32_t hwaddr_read(hwaddr_t addr, size_t len) {
	assert(len == 1 || len == 2 || len == 4);
	return dram_read(addr, len);
}

void hwaddr_write(hwaddr_t addr, size_t len, uint32_t data) {
	assert(len == 1 || len == 2 || len == 4);
	dram_write(addr, len, data);
}

uint32_t swaddr_read(swaddr_t addr, size_t len) {
	assert(len == 1 || len == 2 || len == 4);
	uint32_t cache_data = L1_cache_read(addr, len);
#if 0
	uint32_t hw_data = hwaddr_read(addr, len);
	if(cache_data != hw_data) {
		L1_print(addr);
		int i = 0;
		printf("real ram:\n");
		for (i = 0; i < 64; i ++) {
			printf(" %02x", hwaddr_read((addr&(~63u))+i,1));
			if (i == 31) printf("\n");
		}
		printf("\n");
		test(0, "cache wrong, addr = %x, len = %x, cache = %x, hw = %x", addr, len, cache_data, hw_data);
	}
#endif
	return cache_data;
}

void swaddr_write(swaddr_t addr, size_t len, uint32_t data) {
	assert(len == 1 || len == 2 || len == 4);
	//hwaddr_write(addr, len, data);
	L1_cache_write(addr, len, data);
}

static uint32_t hwaddr_read_instr(hwaddr_t addr, size_t len) {
	assert(len == 1 || len == 2 || len == 4);
	return L1_cache_read(addr, len);
}

uint32_t instr_fetch(swaddr_t addr, size_t len) {
	assert(len == 1 || len == 2 || len == 4);
	return hwaddr_read_instr(addr, len);
}
