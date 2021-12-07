#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>


unsigned rotate_left(unsigned x, int n)
{
    unsigned full1 = -1;
    unsigned w = sizeof(full1) << 3;
    unsigned mask10, mask01, h, l;

    mask01 = full1 >> n;
    mask10 = ~mask01;

    h = x & mask10;
    l = x & mask01;

    return (l << n) | (h >> w-n-1 >> 1);
}


unsigned rotate_left_2(unsigned x, int n)
{
    for (int i = 0; i < n; i++) {
        x = ((x & 0x80000000) >> 31) | (x << 1);
    }

    return x;
}


void test_rotate_left(unsigned x, int n)
{
    if (rotate_left(x, n) != rotate_left_2(x, n)) {
        printf("u=0x%08x, rotate_left:%08x, rotate_left_2:%08x\n",
                x,
                rotate_left(x, n),
                rotate_left_2(x, n));
        abort();
    }
}

// random test
void test()
{
    int seed;
    unsigned r;

    assert(rotate_left(0x12345678, 4) == 0x23456781);
    assert(rotate_left(0x12345678, 20) == 0x67812345);
    assert(rotate_left(0x12345678, 0) == 0x12345678);

    // test INT_MIN
    for (int i = INT_MIN; i < INT_MIN + 100; i++)
        for (int j = 0; j < 32; j++)
            test_rotate_left(i, j);

    // test zero
    for (int i = -256; i <= 256; i++)
        for (int j = 0; j < 32; j++)
            test_rotate_left(i, j);

    // test INT_MAX
    for (int i = INT_MAX - 100; i <= INT_MAX; i++) {
        for (int j = 0; j < 32; j++) {
            test_rotate_left(i, j);
        }
        if (i == INT_MAX)
            break;
    }

    // test random
    seed = time(NULL);
    srandom(seed);
    for (int i = 0; i < 1000000; i++) {
        r = random();
        for (int j = 0; j < 32; j++) {
            // 随机数的大小是0-2147483647
            test_rotate_left(r, j);
        }
    }
}


int main()
{
    test();
}