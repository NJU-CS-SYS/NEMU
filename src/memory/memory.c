#include "common.h"
#include "cpu/segment.h"
#include "cpu/reg.h"
uint32_t cache_read(swaddr_t addr, size_t len);
uint32_t cache_write(swaddr_t addr, size_t len, uint32_t data);
hwaddr_t page_translate(lnaddr_t addr, size_t len);

/* Memory accessing interfaces */

uint32_t hwaddr_read(hwaddr_t addr, size_t len) 
{
	assert(len == 1 || len == 2 || len == 4);
	return cache_read(addr, len);
}

void hwaddr_write(hwaddr_t addr, size_t len, uint32_t data) 
{
	assert(len == 1 || len == 2 || len == 4);
	cache_write(addr, len, data);
}

static uint32_t lnaddr_read(lnaddr_t addr, size_t len) 
{
	assert(len == 1 || len == 2 || len == 4);
	if (cpu.cr0.protect_enable && cpu.cr0.paging) {
		addr = page_translate(addr, len);
	}
	return hwaddr_read(addr, len);
}

static void lnaddr_write(lnaddr_t addr, size_t len, uint32_t data)
{
	assert(len == 1 || len == 2 || len == 4);
	if (cpu.cr0.protect_enable && cpu.cr0.paging) {
		addr = page_translate(addr, len);
	}
	return hwaddr_write(addr, len, data);
}

uint32_t swaddr_read(swaddr_t addr, size_t len) 
{
	assert(len == 1 || len == 2 || len == 4);
	if (PE) {
		addr = segment_translate(addr);
	}
	return lnaddr_read(addr, len);
}

void swaddr_write(swaddr_t addr, size_t len, uint32_t data) 
{
	assert(len == 1 || len == 2 || len == 4);
	if (PE) {
		addr = segment_translate(addr);
	}
	lnaddr_write(addr, len, data);
}

static uint32_t hwaddr_read_instr(hwaddr_t addr, size_t len) 
{
	assert(len == 1 || len == 2 || len == 4);
	return cache_read(addr, len);
}

uint32_t instr_fetch(swaddr_t addr, size_t len) 
{
	assert(len == 1 || len == 2 || len == 4);
	lnaddr_t lnaddr;
	hwaddr_t hwaddr;
	if (PE) {
		Sreg = CS;
		lnaddr = segment_translate(addr);
	} else {
		lnaddr = addr;
	}
	if (cpu.cr0.protect_enable && cpu.cr0.paging) {
		hwaddr = page_translate(lnaddr,len);
	} else {
		hwaddr = lnaddr;
	}
	if (cpu.eip >= 0xc0100f29)
		Log("swaddr %x lnaddr %x hwaddr %x", addr, lnaddr, hwaddr);
	return hwaddr_read_instr(hwaddr, len);
}
