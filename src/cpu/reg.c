#include "nemu.h"

CPU_state cpu;

const char *regsl[] = {"eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi"};
const char *regsw[] = {"ax", "cx", "dx", "bx", "sp", "bp", "si", "di"};
const char *regsb[] = {"al", "cl", "dl", "bl", "ah", "ch", "dh", "bh"};
const char *crs[] = {"cr0", "cr1", "cr2", "cr3"};
const char *segs[] = {"es", "cs", "ss", "ds"};
