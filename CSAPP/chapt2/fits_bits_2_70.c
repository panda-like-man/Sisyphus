#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

int fits_bits(int x, int n)
{
    return ((unsigned)x >> (n-1) >> 1) == 0;
}


int fits_bits_2(int x, int n)
{
    if (x < 0) return n == 32;

    return ((unsigned)x <= ((1 << n-1 << 1) - 1));
}

void test_fits_bits(unsigned x, int n)
{
    if (fits_bits(x, n) != fits_bits_2(x, n)) {
        printf("u=0x%08x, n=%d, fits_bits:%d, fits_bits_2:%d\n",
                x,
                n,
                fits_bits(x, n),
                fits_bits_2(x, n));
        abort();
    }
}

// random test
void test()
{
    int seed;
    unsigned r;

    assert(fits_bits(0x12345678, 32) == 1);
    assert(fits_bits(-1, 31) == 0);
    assert(fits_bits(0, 1) == 1);

    // test INT_MIN
    for (int i = INT_MIN; i < INT_MIN + 100; i++)
        for (int j = 1; j <= 32; j++)
            test_fits_bits(i, j);

    // test zero
    for (int i = -256; i <= 256; i++)
        for (int j = 1; j <= 32; j++)
            test_fits_bits(i, j);

    // test INT_MAX
    for (int i = INT_MAX - 100; i <= INT_MAX; i++) {
        for (int j = 1; j <= 32; j++) {
            test_fits_bits(i, j);
        }
        if (i == INT_MAX)
            break;
    }

    // test random
    seed = time(NULL);
    srandom(seed);
    for (int i = 0; i < 1000000; i++) {
        r = random();
        for (int j = 1; j <= 32; j++) {
            // 随机数的大小是0-2147483647
            test_fits_bits(r, j);
        }
    }
}


int main()
{
    test();
}