#include "common.h"
#include "io/port-io.h"
#include "debug.h"

#define PORT_IO_SPACE_MAX 65536
#define NR_MAP 8

/* "+ 3" is for hacking, see pio_read() below */
uint8_t pio_space[PORT_IO_SPACE_MAX + 3];

typedef struct {
    ioaddr_t low;
    ioaddr_t high;
    pio_callback_t callback;
} PIO_t;

static PIO_t maps[NR_MAP];
static int nr_map = 0;

static void pio_callback(ioaddr_t addr, size_t len, bool is_write) {
    int i;
    for(i = 0; i < nr_map; i ++) {
        if(addr >= maps[i].low && addr + len - 1 <= maps[i].high) {
            printf("pio_callback calling, i: %d, addr %x, is_write:%x\n", i, addr, is_write);
            maps[i].callback(addr, len, is_write);
            return;
        }
    }
}

/* device interface */
void* add_pio_map(ioaddr_t addr, size_t len, pio_callback_t callback) {
    assert(nr_map < NR_MAP);
    assert(addr + len <= PORT_IO_SPACE_MAX);
    maps[nr_map].low = addr;
    maps[nr_map].high = addr + len - 1;
    maps[nr_map].callback = callback;
    printf("add_pio_map: addr:%x, len:%x, low:%x, high:%x, nr_map:%d\n", addr, len, addr, addr+len-1, nr_map);
    nr_map ++;
    return pio_space + addr;
}


/* CPU interface */
// len <-> ~0u >> ((4 - len) << 3)
//  0             0x00000000
//  1             0x000000ff
//  2             0x0000ffff
//  3             0x00ffffff
//  4             0xffffffff
uint32_t __attribute__((optimize("O0"))) pio_read(ioaddr_t addr, size_t len) {
    assert(len == 1 || len == 2 || len == 4);
    assert(addr + len - 1 < PORT_IO_SPACE_MAX);
    uint8_t *pio_buf = pio_space + addr;
    uint32_t result = (pio_buf[3] << 24) | (pio_buf[2] << 16) | (pio_buf[1] << 8) | pio_buf[0];
    uint32_t data = result & (~0u >> ((4 - len) << 3));
    printf("pio_read: addr %x(%08x), len %x, data %x\n", addr, (unsigned int)pio_buf, len, data);
    pio_callback(addr, len, false);
    return data;
}

void pio_write(ioaddr_t addr, size_t len, uint32_t data) {
    assert(len == 1 || len == 2 || len == 4);
    assert(addr + len - 1 < PORT_IO_SPACE_MAX);
    memcpy(pio_space + addr, &data, len);
    pio_callback(addr, len, true);
}
