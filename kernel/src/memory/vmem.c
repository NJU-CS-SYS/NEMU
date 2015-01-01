#include "common.h"
#include "memory.h"
#include <string.h>
#include "my_printf.h"

#define VMEM_ADDR 0xa0000
#define SCR_SIZE (320 * 200)
#define frame_to_pdir_idx(frame) ( ((frame) & 0xffc00) >> 10 )
#define frame_to_ptable_idx(frame) ((frame) & 0x003ff)

/* Use the function to get the start address of user page directory. */
inline PDE* get_updir();

void create_video_mapping() {
	/* TODO: create an identical mapping from virtual memory area 
	 * [0xa0000, 0xa0000 + SCR_SIZE) to physical memory area 
	 * [0xa0000, 0xa0000 + SCR_SIZE) for user program. You may define
	 * some page tables to create this mapping.
	 */
	PDE *pdir = get_updir();
	uint32_t pdir_idx;
	pdir_idx = frame_to_pdir_idx( VMEM_ADDR );
	Log("pdir %x, idx %x", pdir, pdir_idx);
	nemu_assert(0);
}

void video_mapping_write_test() {
	int i;
	uint32_t *buf = (void *)VMEM_ADDR;
	for(i = 0; i < SCR_SIZE / 4; i ++) {
		buf[i] = i;
	}
}

void video_mapping_read_test() {
	int i;
	uint32_t *buf = (void *)VMEM_ADDR;
	for(i = 0; i < SCR_SIZE / 4; i ++) {
		assert(buf[i] == i);
	}
}

void video_mapping_clear() {
	memset((void *)VMEM_ADDR, 0, SCR_SIZE);
}

