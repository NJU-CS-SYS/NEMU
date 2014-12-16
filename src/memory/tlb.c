#include "common.h"
#include "lib/misc.h"
#include "cpu/reg.h"
#include "stdlib.h"
#include "page.h"

#define NR_ENTRY 64
#define TAG_MASK(addr) ( (addr & 0xfffff000) >> 12 )
typedef struct tlb_entry {
	uint32_t valid : 1;
	uint32_t tag   : 20;
	PTE pte;
} TE;

#define TAG(ppte) (ppte->tag)
#define VALID(ppte) (ppte->valid)
TE Tlb[NR_ENTRY];
void tlb_init(void)
{
	memset(Tlb, 0, sizeof(TE) * NR_ENTRY);
}

bool tlb_search(uint32_t tag, int* idx)
{
	int i;
	TE *ptlb = (TE*)Tlb;
	for (i = 0; i < NR_ENTRY; i ++) {
		if (tag == TAG(ptlb) && VALID(ptlb)) {
			(*idx) = i;
			return true;
		}
	}
	return false;
}

bool tlb_read(lnaddr_t addr, hwaddr_t *outaddr)
{
	int i = -1;
	uint32_t tag = TAG_MASK(addr);
	Lnaddr temp;
	temp.val = addr;
	if ( tlb_search(tag, &i) ) {
		test(i != -1, "i should not be -1 when hitting tlb!");
		(*outaddr) =  ( ((Tlb[i].pte.page_frame) << 12) | temp.offset );
		return true;
	}
	
	return false;
}

void tlb_insert(lnaddr_t addr, PTE page)
{
	int i = rand() % NR_ENTRY;
	Tlb[i].valid = 1;
	Tlb[i].tag = TAG_MASK(addr);
	Tlb[i].pte = page;
}
