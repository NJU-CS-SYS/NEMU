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
    uint8_t buf[4096];
    ide_read(buf, ELF_OFFSET_IN_DISK, 4096);
    elf = (void*)buf;
#else
    /* The ELF file is located at memory address 0 */
    elf = (void *)0x0;
#endif

    ph = (void*)((uint8_t*)elf + elf->e_phoff);

    int i, j;
    uint8_t *dest;
    uint32_t filesz;
    uint32_t memsz;
    for (i = 0; i < elf->e_phnum; i++) {
        /* Scan the program header table, loader each segment into memory */
        if (ph->p_type == PT_LOAD) {

            dest = (uint8_t *)ph->p_vaddr;
            filesz = ph->p_filesz;
            memsz = ph->p_memsz;
    
#ifdef IA32_PAGE
            dest = (uint8_t *)mm_malloc((uint32_t)dest, memsz);
            STOP;
#endif

            /* Memory copy */

#ifdef HAS_DEVICE
            ide_read((uint8_t *)dest, ELF_OFFSET_IN_DISK + ph->p_offset, filesz);
            j = filesz;
#else
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

        }
        ph ++;
    }

    volatile uint32_t entry = elf->e_entry;

#ifdef IA32_PAGE
    mm_malloc(KOFFSET - STACK_SIZE, STACK_SIZE);

#ifdef HAS_DEVICE
    create_video_mapping();
#endif

    STOP;
    write_cr3(get_ucr3());
#endif

    return entry;
}
