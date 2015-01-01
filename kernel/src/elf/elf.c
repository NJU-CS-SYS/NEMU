#include "common.h"
#include "memory.h"
#include <string.h>

#include <elf.h>

#ifdef HAS_DEVICE
#define ELF_OFFSET_IN_DISK 0
#endif

#define STACK_SIZE (1 << 20)

void ide_read(uint8_t *, uint32_t, uint32_t);
void create_video_mapping();
uint32_t get_ucr3();

uint32_t loader() {
	Elf32_Ehdr *elf;
	Elf32_Phdr *ph = NULL;

#ifdef HAS_DEVICE
	uint8_t buf[4096];
	ide_read(buf, ELF_OFFSET_IN_DISK, 4096);
	elf = (void*)buf;
#else
	/* The ELF file is located at memory address 0 */
	elf = (void *)0x0;
#endif

#if 0
	/* Load each program segment */
	for(; true; ) {
		/* Scan the program header table, load each segment into memory */
		if(ph->p_type == PT_LOAD) {

			/* TODO: read the content of the segment from the ELF file 
			 * to the memory region [VirtAddr, VirtAddr + FileSiz)
			 */
			 
			 
			/* TODO: zero the memory region 
			 * [VirtAddr + FileSiz, VirtAddr + MemSiz)
			 */


			/* Record the prgram break for future use. */
			extern uint32_t brk;
			uint32_t new_brk = ph->p_vaddr + ph->p_memsz - 1;
			if(brk < new_brk) { brk = new_brk; }
		}
	}
#endif

	ph = (void*)elf->e_phoff;
	uint16_t step = elf->e_phentsize;

	int i;
	for (i = 0; i < elf->e_phnum; i++) {
		/* Scan the program header table, loader each segment into memory */
		if (ph->p_type == PT_LOAD) {
			char *dest = (char*)ph->p_vaddr;
			char *src = (char*)ph->p_offset;
			uint32_t filesz = ph->p_filesz;
			uint32_t memsz = ph->p_memsz;
			int j;
	
#ifdef IA32_PAGE
			dest = (char*)mm_malloc(ph->p_vaddr, ph->p_memsz);
#endif
			/* Memory copy */
			for (j = 0; j < filesz; j++)
				dest[j] = src[j];

			/* Memory clear */
			for (; j < memsz; j++)
				dest[j] = 0;

			/* Record the prgram break for future use. */
			extern uint32_t brk;
			uint32_t new_brk = ph->p_vaddr + ph->p_memsz - 1;
			if(brk < new_brk) { brk = new_brk; }

		}
		ph = (Elf32_Phdr*)((uint32_t)ph + step);
	}

	volatile uint32_t entry = elf->e_entry;

#ifdef IA32_PAGE
	mm_malloc(KOFFSET - STACK_SIZE, STACK_SIZE);

#ifdef HAS_DEVICE
#if 0
	create_video_mapping();
#endif
#endif

	write_cr3(get_ucr3());
#endif

	return entry;
}
