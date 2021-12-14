#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

// 0 <= k < w-1
int divide_power2(int x, int k)
{
    int w = sizeof(int) << 3;
    int mask = 1 << (w - 1);
    int s = !!(x & mask);
    int bais = (s << k) - s;

    // if (x == -3 && k == 1)
    //     printf("s:%d, bais:%d, result:%d\n", s, bais, (x+bais) >> k);

    return (x + bais) >> k;
}

// 0 <= k < w-1
int divide_power2_2(int x, int k)
{
    int y = 1 << k;

    return x / y;
}


void test_divide_power2(int x, int k)
{
    if (divide_power2(x, k) != divide_power2_2(x, k)) {
        printf("u=0x%08x, k=%d, divide_power2:%08x, divide_power2_2:%08x\n",
                x,
                k,
                divide_power2(x, k),
                divide_power2_2(x, k));
        abort();
    }
}

// random test
void test()
{
    int seed;
    unsigned r;

    assert(divide_power2(0, 0) == 0);
    assert(divide_power2(0, 30) == 0);
    assert(divide_power2(1, 5) == 0);
    assert(divide_power2(3, 1) == 1);
    assert(divide_power2(-3, 1) == -1);
    assert(divide_power2(-3, 2) == 0);

    // test INT_MIN
    for (int i = INT_MIN; i < INT_MIN + 100; i++)
        for (int j = 0; j < 31; j++)
            test_divide_power2(i, j);

    // test zero
    for (int i = -256; i <= 256; i++)
        for (int j = 0; j < 31; j++)
            test_divide_power2(i, j);

    // test INT_MAX
    for (int i = INT_MAX - 100; i <= INT_MAX; i++) {
        for (int j = 0; j < 31; j++)
            test_divide_power2(i, j);

        if (i == INT_MAX)
            break;
    }

    // test random
    seed = time(NULL);
    srandom(seed);
    for (int i = 0; i < 1000000; i++) {
        // 随机数的大小是0-2147483647
        r = random();
        for (int j = 0; j < 31; j++)
            test_divide_power2(r, j);
    }
}


int main()
{
    test();
}