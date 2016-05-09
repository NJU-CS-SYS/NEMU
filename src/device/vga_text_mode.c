#include "monitor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

extern Monitor monitor;
extern int global_use_std; // TODO cond-compile it

#define VMEM (monitor.text_buffer)
#define HEIGHT (monitor.height)
#define WIDTH (monitor.width)

static int curr_line = 0;
static int curr_col = 0;

void npc_fputc(char ch, FILE *fp)
{
    if (global_use_std && fp != NULL) {
        fputc(ch, fp);
    }
    else {
        if (curr_line == HEIGHT) {
            // No line for print, it is time to scroll ;-)
            memmove(VMEM, VMEM + sizeof(VMEM[0]) * WIDTH, sizeof(VMEM[0]) * WIDTH * (HEIGHT - 1));
            memset(VMEM + sizeof(VMEM[0]) * WIDTH * (HEIGHT - 1), ' ', WIDTH);
            curr_line--;
        }
        if (ch == '\n') {
            // Use space to mimic new line
            while (curr_col < WIDTH) VMEM[curr_line * WIDTH + curr_col++] = ' ';
        }
        else {
            VMEM[curr_line * WIDTH + curr_col++] = ch;
        }
    }

    if (curr_col == WIDTH) {
        curr_line++;
        curr_col = 0;
    }
}

void npc_fputs_chomp(const char *s, FILE *fp)
{
    while (*s != '\0') {
        npc_fputc(*s++, fp);
    }
}

void npc_fputs(const char *s, FILE *fp)
{
    npc_fputs_chomp(s, fp);
    npc_fputc('\n', fp);
}

void npc_fprintf(FILE *fp, const char *fmt, ...)
{
    char str[1024];
    va_list args;
    va_start(args, fmt);
    vsnprintf(str, sizeof(str), fmt, args);
    va_end(args);
    npc_fputs_chomp(str, fp);
}
