// 用内存模拟文件
// rf -> ram file
#include "common.h"

extern unsigned char testfile[];
extern unsigned int testfile_len;

static int cursor = 0;

// 写操作，不检查错误，针对测试文件
size_t rfwrite(const void *buf, size_t size, size_t nmemb)
{
    const unsigned char *data = buf;
    size_t nr_wr = 0;
    for (size_t i = 0; i < nmemb; i++) {
        for (size_t j = 0; j < size; j++) {
            testfile[cursor++] = *data++;
        }
        nr_wr += size;
    }
    return nr_wr;
}

// 读操作，不检查错误，针对测试文件
size_t rfread(void *buf, size_t size, size_t nmemb)
{
    unsigned char *data = buf;
    size_t nr_rd = 0;
    for (size_t i = 0; i < nmemb; i++) {
        for (size_t j = 0; j < size; j++) {
            *data++ = testfile[cursor++];
        }
        nr_rd += size;
    }
    return nr_rd;
}

// 只有 set 需求！
int rfseek(int index)
{
    cursor = index;
    return cursor;
}

// 如果进行了写文件操作，则不能保证完全保持初始状态
void load_prog(void)
{
    cursor = 0;
}
