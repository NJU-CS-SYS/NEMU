#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <stdio.h>
#include <assert.h>

#include "macro.h"
#include "ui/ui.h"

extern int nemu_state;
void info();

#define test(cond, ...) \
    do { \
        if(!(cond)) { \
            fflush(stdout); \
            fprintf(stderr, __FILE__ "@" str(__LINE__) ": " __VA_ARGS__); \
            info(); \
            nemu_state = TEST_INT; \
        } \
    } while(0)

#define Test(cond, ...) \
    do { \
        if(!(cond)) { \
            fflush(stdout); \
            fprintf(stderr, __FILE__ "@" str(__LINE__) ": " __VA_ARGS__); \
            nemu_state = TEST_INT; \
        } \
    } while(0)

extern int enable_debug;

#define Log(format,...) \
    do { \
        if (enable_debug) { \
            fprintf(stdout, "%s,%s,%d: " format "\n", __FILE__, __func__, __LINE__, ## __VA_ARGS__); \
            fflush(stdout); \
        } \
    } while (0)

#endif
