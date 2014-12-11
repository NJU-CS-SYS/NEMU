#include "common.h"
#include "cpu/reg.h"

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
		uint32_t dir : 10;
		uint32_t page : 10;
		uint32_t offset : 12;
	};
	uint32_t val;
} Lnaddr;
hwaddr_t page_translate(lnaddr_t addr, size_t len)
{
	Lnaddr lnaddr;
	lnaddr.val = addr;
	hwaddr_t dir_addr = cpu.cr3.page_directory_base << 12;
	PDE dir_entry;
	dir_entry.val = hwaddr_read(dir_addr + lnaddr.dir, 4);
	Log("dir_addr %#x", dir_addr);
	Log("page_frame %x", dir_entry.page_frame);
	test(0,"hit");
	return 0;
}
