#include "vga.h"

#include "io/port-io.h"
#include "device/i8259.h"

#define VMEM_DATA_PORT 0x30
static uint8_t *vmem_data_port_base;

void vga_vmem_io_handler(ioaddr_t addr, size_t len, bool is_write) {
  if (is_write) {
    putchar(vmem_data_port_base[0]);
  }
}

void init_vga() {
    vmem_data_port_base = add_pio_map(VMEM_DATA_PORT, 1, vga_vmem_io_handler);
}
