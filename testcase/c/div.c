#define NEMU
#define DIV 4

#ifndef NEMU
#include <assert.h>
#else
#include <trap.h>
#endif

#ifndef NEMU
#define ASSERT assert
#else
#define ASSERT nemu_assert
#endif

static const int test_data[] = { 5, 6, 7, 8, 9, 10 };
static const int answer[]    = { 1, 1, 1, 2, 2, 2  };
static int i = 0;

int main()
{
	for (i = 0; i < 6; i ++)
	{
		ASSERT(answer[i] == test_data[i] / DIV);
	}
	return 0;
}
