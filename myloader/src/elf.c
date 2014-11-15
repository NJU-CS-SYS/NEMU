#include <elf.h>
#include "trap.h"

void loader() {
	/* The ELF file is located at memory address 0 */
	Elf32_Ehdr *elf = (void*)0;

	Elf32_Phdr *ph = (void*)elf->e_phoff;
	uint16_t step = elf->e_phentsize;
	//uint16_t phnum = elf->e_phnum;

	uint16_t i;
	int k = 0;
	nemu_assert(elf->e_phnum == 3);
	for (i = 0; i < elf->e_phnum; i++) {
		/* Scan the program header table, loader each segment into memory */
		if (ph->p_type == PT_LOAD) {
			k++;
			char *dest = (char*)ph->p_vaddr;
			char *src = (char*)ph->p_offset;
			uint32_t filesz = ph->p_filesz;
			uint32_t memsz = ph->p_memsz;
			int j;
			/* Memory copy */
			if (i == 2) { nemu_assert(src[20000] == 27825); }
			for (j = 0; j < filesz; j++)
				dest[j] = src[j];

			/* Memory clear */
			for (; j < memsz; j++)
				dest[j] = 0;
		}
		ph = (Elf32_Phdr*)((uint32_t)ph + step);
	}

	/* Hei wei gou! */
	((void(*)(void))elf->e_entry)();
	/* When returning from the program, it is executed successfully */
	HIT_GOOD_TRAP;
}
