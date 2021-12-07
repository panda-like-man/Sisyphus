#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>


/*
    find the leftmost 1 in x, assume w=32
    ie. 0xff00---->0x8000, 0x6600---->0x4000
    return 0 if x = 0

    原理：首先把x表示为00..0011...11的形式
*/
int leftmost_one(unsigned x)
{
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;

    return x ^ (x >> 1);
}

int leftmost_one_2(unsigned x)
{
    if (x == 0)
        return 0;

    int mask = 0x80000000;

    // 注意运算符的优先级
    // 这里不加括号会出莫名其妙的问题
    while ((x & mask) == 0) {
        mask >>= 1;
    }

    return x & mask;
}


void test_leftmost_one(unsigned x)
{
    if (leftmost_one(x) != leftmost_one_2(x)) {
        printf("u=0x%08x, leftmost_one:%08x, leftmost_one_2:%08x\n",
                x,
                leftmost_one(x),
                leftmost_one_2(x));
        abort();
    }
}

// random test
void test()
{
    int seed;
    unsigned r;

    assert(leftmost_one(0xff00) == 0x8000);
    assert(leftmost_one(0x6600) == 0x4000);
    assert(leftmost_one(0) == 0);

    // test INT_MIN
    for (int i = INT_MIN; i < INT_MIN + 100; i++)
        test_leftmost_one(i);

    // test zero
    for (int i = -256; i <= 256; i++)
        test_leftmost_one(i);

    // test INT_MAX
    for (int i = INT_MAX - 100; i <= INT_MAX; i++) {
        test_leftmost_one(i);
        if (i == INT_MAX)
            break;
    }

    // test random
    seed = time(NULL);
    srandom(seed);
    for (int i = 0; i < 1000000; i++) {
        // 随机数的大小是0-2147483647
        r = random();
        test_leftmost_one(r);
    }
}


int main()
{
    test();
}