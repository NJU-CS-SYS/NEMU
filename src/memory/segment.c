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
	index = index << 3;
	desc.val_low = hwaddr_read(gdt_addr + index, 4);
	desc.val_high = hwaddr_read(gdt_addr + index + 4, 4);

	lnaddr_t base = desc.base_31_24 << 24 | desc.base_23_16 << 16 | desc.base_15_0;
	if (base != 0) {
		Log("gdt %x",gdt_addr);
		Log("base %x", base);
		Log("limit %x  %x", desc.limit_19_16, desc.limit_15_0);
		Log("segment type　%x", desc.segment_type);
		Log("sel %x, index %x", selector, index);
		int i;
		for (i = 0; i < 3; i++) {
			Log(" %02x", hwaddr_read(gdt_addr + index + i, 1));
		}
		for (; i < 8; i ++) {
			Log(" %02x", hwaddr_read(gdt_addr + index + i, 1));
		}

	}
	return base + addr;
}
