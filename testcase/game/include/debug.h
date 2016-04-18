#ifndef __DEBUG_H__
#define __DEBUG_H__

#include "common.h"
#include "x86.h"

#define GAME_LABEL " {game} "


#define Log(format, ...) \
    printf("[%s,%d,%s]" GAME_LABEL format "\n", \
__FILE__, __LINE__, __func__, ## __VA_ARGS__) \

#define panic(format, ...) \
do { \
    cli(); \
    Log("system panic: " format, ## __VA_ARGS__); \
    HIT_BAD_TRAP; \
} while(0)

#define assert(cond) \
do { \
    if(!(cond)) { \
        panic("Assertion failed: %s", #cond); \
    } \
} while(0)
        

#endif
