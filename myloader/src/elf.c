#include <elf.h>
#include "trap.h"

void loader() {
	/* The ELF file is located at memory address 0 */
	Elf32_Ehdr *elf = (void*)0;

	Elf32_Phdr *ph = (void*)elf->e_phoff;
	uint16_t step = elf->e_phentsize;
	//uint16_t phnum = elf->e_phnum;

	uint16_t i;
	for (i = 0; i < elf->e_phnum; i++) {
		/* Scan the program header table, loader each segment into memory */
		if (ph->p_type == PT_LOAD) {
			HIT_GOOD_TRAP;
			char *dest = (char*)ph->p_vaddr;
			char *src = (char*)ph->p_offset;
			uint16_t filesz = ph->p_filesz;
			uint16_t memsz = ph->p_memsz;
			int j;
			/* Memory copy */
			for (j = 0; j < filesz; j++)
				dest[j] = src[j];

			/* Memory clear */
			for (; j < memsz; j++)
				dest[j] = 0;
		}
		ph += step;
	}

	/* Hei wei gou! */
	((void(*)(void))elf->e_entry)();

	nemu_assert(elf->e_phnum != 4);
	/* When returning from the program, it is executed successfully */
	HIT_GOOD_TRAP;
}
