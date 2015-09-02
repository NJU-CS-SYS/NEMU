#include "trap.h"
#define MAX_SIZE 100
#define MOD 91

#define DELETE -1
#define EMPTY 0

unsigned int test_data[MAX_SIZE];
unsigned int answer[MAX_SIZE];

typedef struct {
    int value;
    int ref;
}Element;

Element barrier[MAX_SIZE];

void init_barrier(Element* barrier) 
{
    int i;
    for (i = 0; i < MAX_SIZE; i ++) {
        barrier[i].value = 0;
        barrier[i].ref = 0;
    }
}

void insert_barrier(Element* barrier, int element) 
{
    int dst = element % MOD;
    int i = 1;
    int pos = dst;
    while (barrier[pos].ref != EMPTY) {
    if (barrier[pos].ref == DELETE) break;
    i ++;
    pos = (pos + 1) % MAX_SIZE;
    if (pos == dst) break;
    }

    if (pos == dst && barrier[pos].ref != EMPTY && barrier[pos].ref != DELETE) {
    return;
    }

    barrier[pos].value = element;
    barrier[pos].ref = i;
}

int main() 
{
    int i;
    init_barrier(barrier);
    for (i = 0; i < MAX_SIZE; i ++) {
        test_data[i] = i;
    }
    for (i = 0; i < MAX_SIZE; i ++) {
        nemu_assert(i == test_data[i]);
    }
    return 0;
}
