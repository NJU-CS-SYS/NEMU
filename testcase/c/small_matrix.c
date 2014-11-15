#include "trap.h"

#define N 2
int a[N][N] = {{1,2},
			   {3,4}};
int b[N][N] = {{4,3},
	           {2,1}};
int ans[N][N] = {{8,5},{20,13}};
int c[N][N];
int i, j, k;
int m, n;
int mul;
int main() {
	for(i = 0; i < N; i ++) {
		for(j = 0; j < N; j ++) {
			c[i][j] = 0;
			for(k = 0; k < N; k ++) {
				mul = a[i][k] * b[k][j];
				c[i][j] += mul;
			}
			m = c[i][j];
			n = ans[i][j];
			nemu_assert(c[i][j] == ans[i][j]);
		}
	}

	HIT_GOOD_TRAP;

	return 0;

}
