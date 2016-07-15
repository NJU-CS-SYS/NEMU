#include "trap.h"

int add(int a, int b) {
    int c = a + b;
    return c;
}

int test_data[] = { 0, 1 };
int ans[] = { 0, 1, 1, 2 };

#define NR_DATA (sizeof(test_data) / sizeof(test_data[0]))

int i, j, ans_idx = 0;
int main() {
    for(i = 0; i < NR_DATA; i ++) {
        for(j = 0; j < NR_DATA; j ++) {
            nemu_assert( add(test_data[i], test_data[j]) == ans[ans_idx ++]);
        }
    }

    HIT_GOOD_TRAP;

    return 0;
}
