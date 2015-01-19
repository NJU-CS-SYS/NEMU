#include "common.h"

#ifdef BIN_SCALE

#include "FLOAT.h"

FLOAT F_mul_F(FLOAT a, FLOAT b) {
	return ((a * b) >> 16);
}

FLOAT F_div_F(FLOAT a, FLOAT b) {
	assert(b != 0);
	return ((a / b) << 16);
}

FLOAT f2F(float a) {
	return (int32_t)(a * (1 << 16));
}

static inline FLOAT Fabs(FLOAT x) {
	return (x & 0x7fffffff);
}


FLOAT pow(FLOAT x, FLOAT y) {
	/* we only compute x^(1/3) */
	assert(0);
	FLOAT t, t2, dt, next_t = int2F(2);

	do {
		t = next_t;
		t2 = F_mul_F(t, t);
		dt = (F_div_F(x, t2) - t) / 3;
		next_t = t + dt;
	} while(Fabs(dt) > f2F(1e-3));

	return next_t;
}

#endif
