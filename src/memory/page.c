#include "common.h"
#include "cpu/reg.h"
#include "ui/ui.h"
#include "page.h"

//#define TEST_FREE

#ifdef TEST_FREE
static int trigger = 1;
#endif

uint32_t hwaddr_read(hwaddr_t addr, size_t len);
bool tlb_read(lnaddr_t addr, hwaddr_t *hwaddr);
void tlb_insert(lnaddr_t addr, PTE pte);

hwaddr_t page_translate(lnaddr_t addr, size_t len)
{
    hwaddr_t hwaddr = -1;
    hwaddr_t __hwaddr = -1;
#if 1
    if ( tlb_read(addr, &hwaddr) ) {
        test(hwaddr != -1, "tlb hit and wrong: return %x", hwaddr);
        return hwaddr;
    }
#endif
    
    test(hwaddr == -1, "tlb miss and wrong");
    Lnaddr lnaddr;
    PDE dir_entry;
    PTE page_entry;

    lnaddr.val = addr;

    hwaddr_t dir_addr = cpu.cr3.page_directory_base << 12;

    dir_entry.val = hwaddr_read(dir_addr + 4 * lnaddr.dir, 4);
    if (!dir_entry.present) Test(0, "dir fault: eip %#x, vaddr %#x, dir %#x, page %#x, offset %#x", cpu.eip, addr, lnaddr.dir, lnaddr.page, lnaddr.offset);
    page_entry.val = hwaddr_read((dir_entry.page_frame << 12) + 4 * lnaddr.page, 4);
    if (!page_entry.present) Test(0, "page fault: eip %#x, vaddr %#x, dir %#x, page %#x, offset %#x", cpu.eip, addr, lnaddr.dir, lnaddr.page, lnaddr.offset);
    __hwaddr = (page_entry.page_frame << 12) + lnaddr.offset;

    tlb_insert(addr, page_entry);

#undef TEST_FREE
#ifdef TEST_FREE
    if (trigger) {
        lnaddr.val = 0x81440e0;
        dir_addr = cpu.cr3.page_directory_base << 12;
        dir_entry.val = hwaddr_read(dir_addr + 4 * lnaddr.dir, 4);
        page_entry.val = hwaddr_read((dir_entry.page_frame << 12) + 4 * lnaddr.page, 4);
        if (dir_entry.present & page_entry.present) {
            Test(0, "Hit");
            trigger = 0;
        }
    }
#endif

    return __hwaddr;
}
