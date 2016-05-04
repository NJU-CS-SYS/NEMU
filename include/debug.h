#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <stdio.h>
#include <assert.h>

#define test(cond, ...) \
    do { \
        if(!(cond)) { \
            fflush(stdout); \
            fprintf(stderr, __VA_ARGS__); \
            assert(0); \
        } \
    } while(0)

#define Test(cond, ...) \
    do { \
        if(!(cond)) { \
            fflush(stdout); \
            fprintf(stderr, __VA_ARGS__); \
            nemu_state = TEST_INT;\
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
