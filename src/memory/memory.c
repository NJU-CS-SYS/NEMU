#include "common.h"
#include "cpu/segment.h"
#include "io/mmio.h"
#include "cpu/reg.h"
uint32_t dram_read(swaddr_t addr, size_t len);
uint32_t dram_write(swaddr_t addr, size_t len, uint32_t data);
//hwaddr_t page_translate(lnaddr_t addr, size_t len);

/* Memory accessing interfaces */

uint32_t hwaddr_read(hwaddr_t addr, size_t len)
{
    assert(len == 1 || len == 2 || len == 4);
    /*int mmio_code = is_mmio(addr);
    if (mmio_code != -1) {
        //printf("Read MMIO %x ", mmio_code);
        uint32_t video_data =  mmio_read(addr, len, mmio_code);
        //printf("MMIO Data = %02x\n", (unsigned char)video_data);
        return video_data;
    } else {
        return dram_read(addr, len);
    }*/
    return dram_read(addr, len);
}

void hwaddr_write(hwaddr_t addr, size_t len, uint32_t data)
{
    assert(len == 1 || len == 2 || len == 4);
    /*int mmio_code = is_mmio(addr);
    if (mmio_code != -1) {
        //printf("Write MMIO %x MMIO Data = %02x\n", mmio_code, (unsigned char)data);
        mmio_write(addr, len, data, mmio_code);
    } else {
        dram_write(addr, len, data);
    }*/
    dram_write(addr, len, data);
}

static uint32_t lnaddr_read(lnaddr_t addr, size_t len)
{
    assert(len == 1 || len == 2 || len == 4);
    /*if (cpu.cr0.protect_enable && cpu.cr0.paging) {
        addr = page_translate(addr, len);
    }*/
    return hwaddr_read(addr, len);
}

static void lnaddr_write(lnaddr_t addr, size_t len, uint32_t data)
{
    assert(len == 1 || len == 2 || len == 4);
    /*if (cpu.cr0.protect_enable && cpu.cr0.paging) {
        addr = page_translate(addr, len);
    }*/
    return hwaddr_write(addr, len, data);
}

uint32_t swaddr_read(swaddr_t addr, size_t len)
{
    assert(len == 1 || len == 2 || len == 4);
    return lnaddr_read(addr, len);
}

void swaddr_write(swaddr_t addr, size_t len, uint32_t data)
{
    assert(len == 1 || len == 2 || len == 4);
    lnaddr_write(addr, len, data);
}

static uint32_t hwaddr_read_instr(hwaddr_t addr, size_t len)
{
    assert(len == 1 || len == 2 || len == 4);
    return dram_read(addr, len);
}

uint32_t instr_fetch(swaddr_t addr, size_t len)
{
    assert(len == 1 || len == 2 || len == 4);
    hwaddr_t hwaddr;
    /*if (cpu.cr0.protect_enable && cpu.cr0.paging) {
        hwaddr = page_translate(addr,len);
    } else {
        hwaddr = addr;
    }*/
    hwaddr = addr;
    return hwaddr_read_instr(hwaddr, len);
}
