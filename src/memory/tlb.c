#include "common.h"
#include "lib/misc.h"
#include "cpu/reg.h"
#include "stdlib.h"
#include "page.h"

#define NR_ENTRY 64
typedef struct tlb_entry {
	uint32_t valid : 1;
	PTE pte;
} TE;
