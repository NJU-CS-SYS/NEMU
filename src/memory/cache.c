#include "common.h"

#define READ L1_cache_read
#define WRITE L1_cache_write
uint32_t READ(swaddr_t addr, size_t len);
void WRITE(swaddr_t addr, size_t len, uint32_t data);

uint64_t mem_access = 0;
uint32_t cache_read(swaddr_t addr, size_t len) {
	mem_access ++;
	return READ(addr, len);
}

void cache_write(swaddr_t addr, size_t len, uint32_t data) {
	mem_access ++;
	WRITE(addr, len, data);
}
