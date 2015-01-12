#include "common.h"
#include "memory.h"
#include <string.h>

#include <elf.h>

#ifdef HAS_DEVICE
#define ELF_OFFSET_IN_DISK 0
#define BUF_LEN 4096
#endif

#define STACK_SIZE (1 << 20)

void ide_read(uint8_t *, uint32_t, uint32_t);
void create_video_mapping();
uint32_t get_ucr3();

uint32_t loader() {
	Elf32_Ehdr *elf;
	Elf32_Phdr *ph = NULL;

#ifdef HAS_DEVICE
	Log("loading elf head...");
	uint8_t buf[4096];
	ide_read(buf, ELF_OFFSET_IN_DISK, 4096);
	elf = (void*)buf;
	Log("elf head is loaded");
#else
	/* The ELF file is located at memory address 0 */
	elf = (void *)0x0;
#endif

	ph = (void*)(elf + elf->e_phoff);
	uint16_t step = elf->e_phentsize;

	Log("phoff %x", elf->e_phoff);
	Log("phnum %x", elf->e_phnum);
	Log("phsize %x", elf->e_phentsize);

	int i;
	for (i = 0; i < elf->e_phnum; i++) {
		/* Scan the program header table, loader each segment into memory */
		Log("phvaddr %x", ph->p_vaddr);
		if (ph->p_type == PT_LOAD) {
			Log("phvaddr %x", ph->p_vaddr);
			char *dest = (char*)ph->p_vaddr;
			uint32_t filesz = ph->p_filesz;
			uint32_t memsz = ph->p_memsz;
	
#ifdef IA32_PAGE
			dest = (char*)mm_malloc(ph->p_vaddr, ph->p_memsz);
#endif
			/* Memory copy */
#ifdef HAS_DEVICE
			uint8_t section[BUF_LEN];
			int times = filesz / BUF_LEN;
			int rest = filesz % BUF_LEN;
			int time, k, j = 0;

			for (time = 0; time < times; time ++) { // load disk data one buf a time
				ide_read(section, 
						 ELF_OFFSET_IN_DISK + ph->p_offset + time * BUF_LEN,
						 BUF_LEN);
				for (k = 0; k < BUF_LEN; k ++) {
					dest[j] = section[k];
					j ++;
				}
			}
			
			// load unaligned rest data
			ide_read(section, ELF_OFFSET_IN_DISK + ph->p_offset + time * BUF_LEN, rest);
			for (k = 0; k < rest; k ++) {
				dest[j] = section[k];
				j ++;
			}

			nemu_assert(j == filesz);
#else
			int j;
			char *src = (char*)ph->p_offset;
			for (j = 0; j < filesz; j++)
				dest[j] = src[j];
#endif

			/* Memory clear */
			for (; j < memsz; j++)
				dest[j] = 0;

			/* Record the prgram break for future use. */
			extern uint32_t brk;
			uint32_t new_brk = ph->p_vaddr + ph->p_memsz - 1;
			if(brk < new_brk) { brk = new_brk; }

			Log("a section is loaded");

		}
		ph = (Elf32_Phdr*)((uint32_t)ph + step);
	}

	volatile uint32_t entry = elf->e_entry;

#ifdef IA32_PAGE
	mm_malloc(KOFFSET - STACK_SIZE, STACK_SIZE);

#ifdef HAS_DEVICE
	create_video_mapping();
#endif

	write_cr3(get_ucr3());
#endif

	return entry;
}
