#include "common.h"

static uint64_t key_cnt;
void kb_handle(void);
void add_irq_handle(int, void(*)(void));

void
init_kb(void) {
    key_cnt = 0;
    add_irq_handle(1, kb_handle);
}

void
kb_handle(void)
{
    key_cnt ++;
}
