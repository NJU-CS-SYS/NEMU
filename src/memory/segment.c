#include "common.h"
#include "cpu/segment.h"
#include "cpu/reg.h"

int Sreg;

lnaddr_t segment_translate(swaddr_t addr)
{
	test(PE, "Unexpected real mode");
	hwaddr_t gdt_addr = cpu.gdtr.base;
	uint16_t selector = SEG(Sreg);
	uint16_t index = selector >> 3;
	SegDesc descriptor;
	uint32_t temp[2];
	temp[0] = hwaddr_read(gdt_addr + index, 4);
	temp[1] = hwaddr_read(gdt_addr + index + 4, 4);
	memcpy(&descriptor, temp, sizeof(SegDesc));
	
	lnaddr_t base = descriptor.base_31_24 << 24 | descriptor.base_23_16 << 16 | descriptor.base_15_0;
	if (base != 0)
		Log("base %x", base);
	return base + addr;
}
