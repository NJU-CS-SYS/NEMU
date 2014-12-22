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
	SegDesc desc;
	desc.val_low = hwaddr_read(gdt_addr + index * sizeof(SegDesc), 4);
	desc.val_high = hwaddr_read(gdt_addr + index * sizeof(SegDesc) + 32, 4);

	lnaddr_t base = desc.base_31_24 << 24 | desc.base_23_16 << 16 | desc.base_15_0;
	if (base != 0) {
		Log("base %x", base);
		Log("limit %x  %x", desc.limit_19_16, desc.limit_15_0);
		Log("segment type　%x", desc.segment_type);
		Log("sel %x, index %x", selector, index);
	}
	return base + addr;
}
