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

    if ( tlb_read(addr, &hwaddr) ) {
        test(hwaddr != -1, "tlb hit and wrong: return %x", hwaddr);
        return hwaddr;
    }
    
    test(hwaddr == -1, "tlb miss and wrong");
    Lnaddr lnaddr;
    PDE dir_entry;
    PTE page_entry;

    lnaddr.val = addr;

    hwaddr_t dir_addr = cpu.cr3.page_directory_base << 12;

    dir_entry.val = hwaddr_read(dir_addr + 4 * lnaddr.dir, 4);
    if (!dir_entry.present) {
        Test(0, "dir fault: eip 0x%x, vaddr 0x%x, dir 0x%x, page 0x%x, offset 0x%x", cpu.eip, addr, lnaddr.dir, lnaddr.page, lnaddr.offset);
        assert(0);
    }
    page_entry.val = hwaddr_read((dir_entry.page_frame << 12) + 4 * lnaddr.page, 4);
    if (!page_entry.present) {
        Test(0, "page fault: eip 0x%x, vaddr 0x%x, dir 0x%x, page 0x%x, offset 0x%x", cpu.eip, addr, lnaddr.dir, lnaddr.page, lnaddr.offset);
        assert(0);
    }
    __hwaddr = (page_entry.page_frame << 12) + lnaddr.offset;

    tlb_insert(addr, page_entry);

    return __hwaddr;
}
