#ifdef SYS_LAB
#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>

#include "monitor.h"

extern Monitor monitor;
extern int global_use_std; // TODO cond-compile it

#define VMEM (monitor.text_buffer)
#define HEIGHT (monitor.height)
#define WIDTH (monitor.width)

static int curr_line = 0;
static int curr_col = 0;

void npc_fputc(char ch, FILE *fp)
{
    // Backup the global vars to avoid async side-effects,
    // which may make (line * WIDTH + col) exceed buffer boundary.
    int local_line = curr_line;
    int local_col = curr_col;

#ifndef DEPLOY
    // When we compile NEMU to mips code,
    // we can never output to stdout.
    if (global_use_std) {
        write(2, &ch, sizeof(ch));
    }
    else
#endif
    {

        if (local_line == HEIGHT) {
            // No line for print, it is time to scroll ;-)
            memmove(VMEM, VMEM + sizeof(VMEM[0]) * WIDTH, sizeof(VMEM[0]) * WIDTH * (HEIGHT - 1));
            memset(VMEM + sizeof(VMEM[0]) * WIDTH * (HEIGHT - 1), ' ', WIDTH);
            local_line--;
        }
        if (ch == '\n') {
            // Use space to mimic new line
            while (local_col < WIDTH) VMEM[local_line * WIDTH + local_col++] = ' ';
        }
        else {
            VMEM[local_line * WIDTH + local_col++] = ch;
        }
    }

    if (local_col == WIDTH) {
        local_line++;
        local_col = 0;
    }

    curr_line = local_line;
    curr_col = local_col;
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

#endif
