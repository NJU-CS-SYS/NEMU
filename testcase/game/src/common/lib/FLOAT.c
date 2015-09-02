#include "common.h"

#ifdef BIN_SCALE

#include "FLOAT.h"

typedef union 
{
    struct
    {
        int32_t low;
        int32_t high;
    };
    int64_t val;
} FLOAT_MUL;

FLOAT F_mul_F(FLOAT a, FLOAT b) {
    FLOAT_MUL temp_rst;
    temp_rst.val = (int64_t)a * (int64_t)b;
    
    int32_t rst = ((uint32_t)temp_rst.low) >> 16;
    rst |= (temp_rst.high << 16);
    return rst;
}

FLOAT F_div_F(FLOAT a, FLOAT b) {
    // d0000 / 40000 = 34000
    assert(b != 0);
    int mod = a % b;
    // mod = 00010000
    int integer = a / b;
    // integer = 3;
    int rst = (integer << 16) + mod / (b >> 16);
    
    // 30000 + 10000
    return rst;
}

static inline FLOAT Fabs(FLOAT x) {
    if (x < 0)
        return -x;
    else
        return x;
}

FLOAT pow(FLOAT x, FLOAT y) {
    /* we only compute x^(1/3) */

    FLOAT t, t2, dt, next_t = int2F(2);

    do {
        t = next_t;
        t2 = F_mul_F(t, t);
        int temp = F_div_F(x, t2);
        dt = (temp - t) / 3;
        next_t = t + dt;
        Log("t %x, t2 %x, temp %x, |dt| %x, next_t %x", t, t2, temp, Fabs(dt), next_t);
    } while(Fabs(dt) > 0x41);
    /* TODO: f2F ?
     * 0.001 * (2 ^ 16) = 65 = 0x41
     */

    return next_t;
}

#endif
