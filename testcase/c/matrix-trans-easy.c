#include "trap.h"
#define M 3
#define N 3
int a[M][N] = {
    {1, 2, 3},
    {1, 0, 1},
    {3, 2, 1}
};
int ans[M][N] = {
    {1, 1, 3},
    {2, 0, 2},
    {3, 1, 1}
};
int b[M][N];
int count = 0;
int main()
{
    int i, j;
     for (i = 0; i < N; i++) {
         for (j = 0; j < M; j++) {
            count ++;
            b[j][i] = a[i][j];
            nemu_assert(b[j][i] == ans[j][i]);
        }
    }

    HIT_GOOD_TRAP;

    return 0;
}
