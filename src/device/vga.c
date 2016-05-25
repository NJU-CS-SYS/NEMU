#include "vga.h"

#include "io/port-io.h"
#include "device/i8259.h"

#define VMEM_DATA_PORT 66
static uint8_t *vmem_data_port_base;
extern void npc_fputc(char, FILE *); // to monitor

void vga_vmem_io_handler(ioaddr_t addr, size_t len, bool is_write) {
  if (is_write) {
    npc_fputc(vmem_data_port_base[0], NULL); 
  }
}

void init_vga() {
    vmem_data_port_base = add_pio_map(VMEM_DATA_PORT, 1, vga_vmem_io_handler);
}
