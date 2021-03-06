#include "trap.h"

int test_i;
int test_j;
long long result;
int total;
long long add(long long a, long long b) {
    long long c = a + b;
    return c;
}

long long test_data[] = {0, 1, 2, 0x7fffffffffffffffL, 0x8000000000000000L, 0x8000000000000001L, 0xfffffffffffffffeL, 0xffffffffffffffffL};
long long ans[] = {0L, 0x1L, 0x2L, 0x7fffffffffffffffL, 0x8000000000000000L, 0x8000000000000001L, 0xfffffffffffffffeL, 0xffffffffffffffffL, 0x1L, 0x2L, 0x3L, 0x8000000000000000L, 0x8000000000000001L, 0x8000000000000002L, 0xffffffffffffffffL, 0L, 0x2L, 0x3L, 0x4L, 0x8000000000000001L, 0x8000000000000002L, 0x8000000000000003L, 0L, 0x1L, 0x7fffffffffffffffL, 0x8000000000000000L, 0x8000000000000001L, 0xfffffffffffffffeL, 0xffffffffffffffffL, 0L, 0x7ffffffffffffffdL, 0x7ffffffffffffffeL, 0x8000000000000000L, 0x8000000000000001L, 0x8000000000000002L, 0xffffffffffffffffL, 0L, 0x1L, 0x7ffffffffffffffeL, 0x7fffffffffffffffL, 0x8000000000000001L, 0x8000000000000002L, 0x8000000000000003L, 0L, 0x1L, 0x2L, 0x7fffffffffffffffL, 0x8000000000000000L, 0xfffffffffffffffeL, 0xffffffffffffffffL, 0L, 0x7ffffffffffffffdL, 0x7ffffffffffffffeL, 0x7fffffffffffffffL, 0xfffffffffffffffcL, 0xfffffffffffffffdL, 0xffffffffffffffffL, 0L, 0x1L, 0x7ffffffffffffffeL, 0x7fffffffffffffffL, 0x8000000000000000L, 0xfffffffffffffffdL, 0xfffffffffffffffeL};

#define NR_DATA (sizeof(test_data) / sizeof(test_data[0]))

int main() {
    total = NR_DATA;
    int i, j, ans_idx = 0;
    for(i = 0; i < NR_DATA; i ++) {
        test_i = i;
        for(j = 0; j < NR_DATA; j ++) {
            test_j = j;
            result = add(test_data[i], test_data[j]);
            nemu_assert(result == ans[ans_idx ++]);
        }
    }

    HIT_GOOD_TRAP;

    return 0;
}
