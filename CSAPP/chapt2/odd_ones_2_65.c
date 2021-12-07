#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <time.h>

int count_ones(unsigned x)
{
    int result = (x & 0x55555555) + ((x >> 1) & 0x55555555);
    result = (result & 0x33333333) + ((result >> 2) & 0x33333333);
    result = (result & 0x0f0f0f0f) + ((result >> 4) & 0x0f0f0f0f);
    result = (result & 0x00ff00ff) + ((result >> 8) & 0x00ff00ff);
    result = (result & 0x0000ffff) + ((result >> 16) & 0x0000ffff);

    return result;
}

int count_ones_2(unsigned x)
{
    int sum = 0;

    while (x) {
        if (x & 1) {
            sum += 1;
        }
        x >>= 1;
    }

    return sum;
}

int odd_ones(unsigned x)
{
    return count_ones(x) & 1;
}

int odd_ones_2(unsigned x)
{
    int result;
    result = (x) ^ (x >> 16);
    result = (result) ^ (result >> 8);
    result = (result) ^ (result >> 4);
    result = (result) ^ (result >> 2);
    result = (result) ^ (result >> 1);

    return result & 1;
}

void test_odd_ones(unsigned x)
{
    printf("u=0x%08x, count_ones:%d, count_ones_2:%d\n",
        x,
        count_ones(x),
        count_ones_2(x));

    printf("u=0x%08x, odd_ones:%d, odd_ones_2:%d\n",
            x,
            odd_ones(x),
            odd_ones_2(x));

    if (count_ones(x) != count_ones_2(x)) {
        printf("u=0x%08x, count_ones:%d, count_ones_2:%d\n",
                x,
                count_ones(x),
                count_ones_2(x));
        abort();
    }

    if (odd_ones(x) != odd_ones_2(x)) {
        printf("u=0x%08x, odd_ones:%d, odd_ones_2:%d\n",
                x,
                odd_ones(x),
                odd_ones_2(x));
        abort();
    }
}

// random test
void test()
{
    int seed;
    unsigned r;

    // test INT_MIN
    for (int i = INT_MIN; i < INT_MIN + 100; i++)
        test_odd_ones(i);

    // test zero
    for (int i = -256; i <= 256; i++)
        test_odd_ones(i);

    // test INT_MAX
    for (int i = INT_MAX - 100; i <= INT_MAX; i++) {
        test_odd_ones(i);
        if (i == INT_MAX)
            break;
    }

    // test random
    seed = time(NULL);
    srandom(seed);
    for (int i = 0; i < 100; i++) {
        // 随机数的大小是0-2147483647
        r = random();
        test_odd_ones(r);
    }

}


int main()
{
    test();
}