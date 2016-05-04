#include "ui/ui.h"
#include "nemu.h"

#ifndef DEPLOY

#include <setjmp.h>
jmp_buf jbuf;    /* Make it easy to perform exception handling */

#else

#define setjmp(...)

#endif

#define LOADER_START 0x200000

int exec(swaddr_t);
void init_dram();
void tlb_init();
uint32_t i8259_query_intr();
void i8259_ack_intr();
void raise_intr(uint8_t);

char assembly[40];
char *asm_string = (char *)assembly;

extern uint8_t loader [];
extern uint32_t loader_len;
extern int quiet;

void restart()
{
    /* Perform some initialization to restart a program */
    memcpy(hwa_to_va(LOADER_START), loader, loader_len);
    tlb_init();

    /* General initialization */
    cpu.eip = LOADER_START;
    cpu.ebp = 0;
    cpu.esp = 0x8000000;
    cpu.eflags = 0x2;
    cpu.eax = 0;
    cpu.ecx = 0;
    cpu.edx = 0;
    cpu.cr0.paging = 0;
    cpu.cr0.protect_enable = 0;
    cpu.INTR = 0;
    FLAG_CHG(IF, 0);

    /* Segment initialization */
    cpu.gdtr.limit = 0;
    cpu.gdtr.base = 0;
    cpu.cr[0] = 0; // Set PE to 0

    init_dram();
}

void print_bin_instr(swaddr_t eip, int len)
{
    printf("%8x:   ", eip);
    for(int i = 0; i < len; i ++)
        printf("%02x ", swaddr_read(eip + i, 1));
    printf("%*.s", 50 - (12 + 3 * len), "");
}

int int_polling();

void cpu_exec(volatile uint32_t n)
{
    volatile uint32_t n_temp = n;

    setjmp(jbuf);

    for(; n > 0; n --) {
        swaddr_t eip_temp = cpu.eip;
        
        int instr_len = exec(cpu.eip);

        cpu.eip += instr_len;
        
        if (n_temp != -1 || (enable_debug && !quiet)) {
            print_bin_instr(eip_temp, instr_len);
            puts(assembly);
        }

        int_polling();

        switch(nemu_state) {
            case INT:
                printf("\n\nUser interrupt\n");
            case END:
                return;
            case TEST_INT:
                printf("---Stop to confirm, press `c' to continue---\n");
                return;
        }
    }
}

int int_polling()
{
    if(cpu.INTR & FLAG_VAL(IF)) {
        uint32_t intr_no = i8259_query_intr();
        i8259_ack_intr();
        raise_intr(intr_no);
        return 0;
    }
    return 1;
}
