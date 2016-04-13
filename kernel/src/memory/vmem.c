#include "common.h"
#include "memory.h"
#include <string.h>
#include "my_printf.h"
#include "vga.h"

#define va_to_pdir_idx(va) (((va) & 0xffc00000) >> 22)
#define va_to_ptable_idx(va) (((va) & 0x003ff000) >> 12)
#define pa_to_page_frame(pa) (((pa) & 0xfffff000) >> 12)

/* Use the function to get the start address of user page directory. */
PDE* get_updir();
static PTE vmem_ptable[ SCR_SIZE / PAGE_SIZE ] align_to_page;   // video memory page tables
void create_video_mapping() {
    /* TODO: create an identical mapping from virtual memory area 
     * [0xa0000, 0xa0000 + SCR_SIZE) to physical memory area 
     * [0xa0000, 0xa0000 + SCR_SIZE) for user program. You may define
     * some page tables to create this mapping.
     */
    PDE *pdir = get_updir();
    PTE *ptable = (PTE *)vmem_ptable;

    uint32_t pdir_idx, ptable_idx, pframe_idx;
    uint32_t nr_pdir, nr_page = 0;

    pdir_idx = va_to_pdir_idx( VMEM_ADDR );
    ptable_idx = va_to_ptable_idx( VMEM_ADDR );
    pframe_idx = pa_to_page_frame( VMEM_ADDR );

    uint32_t pframe_idx_max = pa_to_page_frame( VMEM_ADDR + SCR_SIZE );

    /* the 1st page talbe is special */
#if 1
    pdir[pdir_idx ++].val = make_vmem_pde(ptable);
    ptable += ptable_idx;
    for (; ptable_idx < NR_PTE; ptable_idx ++) {

        if (pframe_idx > pframe_idx_max) return;

        ptable->val = make_vmem_pte(pframe_idx << 12);

        nr_page ++;

        pframe_idx ++;
        ptable ++;
    }
#endif


    if (pframe_idx > pframe_idx_max) return;

    for (nr_pdir = 1; nr_pdir <= SCR_SIZE / PT_SIZE; nr_pdir ++) {
        if (pframe_idx > pframe_idx_max) return;
        pdir[pdir_idx ++].val = make_vmem_pde(ptable);
        for (ptable_idx = 0; ptable_idx < NR_PTE; ptable_idx ++) {
            ptable->val = make_vmem_pte(pframe_idx << 12);
            
            nr_page ++;

            pframe_idx ++;
            ptable ++;
        }
    }
}

void video_mapping_write_test() {
    uint32_t i;
    uint8_t *buf = (void *)VMEM_ADDR;
    for(i = 0; i < SCR_SIZE; i ++) {
        buf[i] = i;
    }
}

void video_mapping_read_test() {
    uint32_t i;
    uint8_t *buf = (void *)VMEM_ADDR;
    for(i = 0; i < SCR_SIZE; i ++) {
        assert(buf[i] == (uint8_t)i);
    }
}

void video_mapping_clear() {
    uint32_t i;
    uint8_t *vmem_base = (uint8_t*)VMEM_ADDR;
    Log("in video_mapping_clear");

    for(i = 0; i < SCR_SIZE; i ++) {
        vmem_base[i] = 0;
    }
}

