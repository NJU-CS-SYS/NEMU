#include "common.h"
#include "cpu/reg.h"
#include "ui/ui.h"
#include "page.h"
uint32_t hwaddr_read(hwaddr_t addr, size_t len);
bool tlb_read(lnaddr_t addr, hwaddr_t *hwaddr);
void tlb_insert(lnaddr_t addr, PTE pte);
hwaddr_t page_translate(lnaddr_t addr, size_t len)
{
	hwaddr_t hwaddr = -1;
	hwaddr_t __hwaddr = -1;
#if 1
	if ( tlb_read(addr, &hwaddr) ) {
		test(hwaddr == -1, "tlb hit and wrong: return %x", hwaddr);
	}
#endif
	
	test(hwaddr == -1, "tlb miss and wrong");
	Lnaddr lnaddr;
	PDE dir_entry;
	PTE page_entry;

	lnaddr.val = addr;

	//uint32_t bound = lnaddr.offset + len - 1;
	//if (bound > 0xfffu) Test(0,"override: bound %#x, offset %#x, len %d\n eip %#X vaddr %#X", bound, lnaddr.offset, len, cpu.eip, addr);
	hwaddr_t dir_addr = cpu.cr3.page_directory_base << 12;
	dir_entry.val = hwaddr_read(dir_addr + 4 * lnaddr.dir, 4);
	if (!dir_entry.present) Test(0, "dir fault: eip %#x, vaddr %#x", cpu.eip, addr);
	page_entry.val = hwaddr_read((dir_entry.page_frame << 12) + 4 * lnaddr.page, 4);
	if (!page_entry.present) Test(0, "page fault: eip %#x, vaddr %#x", cpu.eip, addr);
	__hwaddr = (page_entry.page_frame << 12) + lnaddr.offset;
	
	//test(__hwaddr == hwaddr, "__hwaddr %x, hwaddr %x", __hwaddr, hwaddr);
#if 0
	Log("dir %x page table %x page %x", dir_addr, dir_entry.page_frame, page_entry.page_frame);
	Log("lnaddr dir %x, page %x, offset %x", lnaddr.dir, lnaddr.page, lnaddr.offset);
	Log("bit %02x", hwaddr_read(hwaddr, 1));
#endif
	tlb_insert(addr, page_entry);
	return __hwaddr;
}
