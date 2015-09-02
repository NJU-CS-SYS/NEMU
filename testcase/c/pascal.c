#include "trap.h"
//#include "stdio.h"

#define N 31

int a[N][N];
int ans[] = {1, 30, 435, 4060, 27405, 142506, 593775, 2035800, 5852925, 14307150, 30045015, 54627300, 86493225, 119759850, 145422675, 155117520, 145422675, 119759850, 86493225, 54627300, 30045015, 14307150, 5852925, 2035800, 593775, 142506, 27405, 4060, 435, 30, 1};
int test_j;
int test_a;
int main() {
    int i, j;
    for(i = 0; i < N; i ++) {
        a[i][0] = a[i][i] = 1;
    }

    for(i = 2; i < N; i ++) {
        for(j = 1; j < i; j ++) {
            a[i][j] = a[i - 1][j - 1] + a[i - 1][j];
            nemu_assert(a[i][j] > 0);
        }
    }

    for(j = 0; j <= 30; j ++) {
        test_j = j;
        test_a = a[30][j];
        nemu_assert(a[30][j] == ans[j]);
    }

    HIT_GOOD_TRAP;

    return 0;
}
