#include "monitor.h"
#include <stdio.h>
#include <stdarg.h>

extern Monitor monitor;
extern int global_use_std; // TODO cond-compile it

#define VMEM (monitor.text_buffer)
#define HEIGHT (monitor.height)
#define WIDTH (monitor.width)

//static int curr_line = 0;
static int curr_col = 0;

void npc_fputc(char ch, FILE *fp)
{
    if (global_use_std) {
        fputc(ch, fp);
    }
    else {
        fputc(ch, fp);
        VMEM[curr_col++] = ch;
    }
}

void npc_fputs(const char *s, FILE *fp)
{
    while (*s != '\0') {
        npc_fputc(*s++, fp);
    }
    npc_fputc('\n', fp);
}

void npc_fprintf(FILE *fp, const char *fmt, ...)
{
    char str[1024];
    va_list args;
    va_start(args, fmt);
    vsnprintf(str, sizeof(str), fmt, args);
    va_end(args);
    npc_fputs(str, fp);
}
