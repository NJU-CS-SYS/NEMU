#include "common.h"
#include "cpu/reg.h"
#include "ui/ui.h"
uint32_t hwaddr_read(hwaddr_t addr, size_t len);
/* the 32bit Page Directory(first level page table) data structure */
typedef union PageDirectoryEntry {
	struct {
		uint32_t present             : 1;
		uint32_t read_write          : 1;
		uint32_t user_supervisor     : 1;
		uint32_t page_write_through  : 1;
		uint32_t page_cache_disable  : 1;
		uint32_t accessed            : 1;
		uint32_t pad0                : 6;
		uint32_t page_frame          : 20;
	};
	uint32_t val;
} PDE;


/* the 32bit Page Table Entry(second level page table) data structure */
typedef union PageTableEntry {
	struct {
		uint32_t present             : 1;
		uint32_t read_write          : 1;
		uint32_t user_supervisor     : 1;
		uint32_t page_write_through  : 1;
		uint32_t page_cache_disable  : 1;
		uint32_t accessed            : 1;
		uint32_t dirty               : 1;
		uint32_t pad0                : 1;
		uint32_t global              : 1;
		uint32_t pad1                : 3;
		uint32_t page_frame          : 20;
	};
	uint32_t val;
} PTE;

typedef union __Lnaddr {
	struct {
		uint32_t offset : 12;
		uint32_t page : 10;
		uint32_t dir : 10;
	};
	uint32_t val;
} Lnaddr;
hwaddr_t page_translate(lnaddr_t addr, size_t len)
{
	Lnaddr lnaddr;
	PDE dir_entry;
	PTE page_entry;

	lnaddr.val = addr;

	uint32_t bound = lnaddr.offset + len - 1;
	if (bound > 0xfffu) test(0,"override: bound %#x, offset %#x, len %d", bound, lnaddr.offset, len);
	hwaddr_t dir_addr = cpu.cr3.page_directory_base << 12;
	dir_entry.val = hwaddr_read(dir_addr + 4 * lnaddr.dir, 4);
	page_entry.val = hwaddr_read((dir_entry.page_frame << 12) + 4 * lnaddr.page, 4);
	hwaddr_t hwaddr = (page_entry.page_frame << 12) + lnaddr.offset;
#if 0
	Log("dir %x page table %x page %x", dir_addr, dir_entry.page_frame, page_entry.page_frame);
	Log("lnaddr dir %x, page %x, offset %x", lnaddr.dir, lnaddr.page, lnaddr.offset);
	Log("bit %02x", hwaddr_read(hwaddr, 1));
#endif
	return hwaddr;
}
