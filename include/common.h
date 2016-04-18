#ifndef __COMMON_H__
#define __COMMON_H__

#include "debug.h"
#include "macro.h"

#define SYS_LAB

#ifdef SYS_LAB

// Overload the print functions.

#define fprintf npc_fprintf

void npc_fprintf(FILE *file, const char *fmt, ...);
void npc_fputc(char ch, FILE *fp);
void npc_fputs(const char *s, FILE *fp);

#ifdef printf
#undef printf
#endif

#define printf(fmt, ...) npc_fprintf(stdout, fmt, ## __VA_ARGS__)

#define fputs npc_fputs
#define fputc npc_fputc

#ifdef putc
#undef putc
#endif

#define putc(ch, file) npc_fputc(ch, file)

#ifdef puts
#undef puts
#endif

#define puts(s) npc_fputs(s, stdout)

#endif

#include <stdint.h>
#include <assert.h>
#include <string.h>

typedef uint8_t bool;
typedef uint32_t hwaddr_t;
typedef uint32_t lnaddr_t;
typedef uint32_t swaddr_t;
typedef int16_t ioaddr_t;


/* boolean */
#define false 0
#define true 1

#endif
