#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>


// 注意舍入误差
// 舍入误差只和符号位和低两位相关
// 是负数并且低两位有值才需要考虑舍入
// 进行大量的测试来验证
int threefourths(int x)
{
    int h, l, r;
    int mask = 1 << 31;
    int s = !!(x & mask);

    h = x & ~3;
    l = x & 3;
    (l == 0) && (s = 0);

    h = (h >> 2) + (h >> 1);
    l = (l << 1) + l;
    l = l >> 2;

    return h + l + s;
}


// 0 <= k < w-1
// 丑陋的实现，主要的原理是把负数变为正数再计算
// 同时需要注意舍入位
// 同时注意Tmin没有对应的正数
// int threefourths(int x)
// {
//     int h, l, r;
//     int mask = 1 << 31;
//     int s = !!(x & mask);

//     r = x;
//     (s == 1 && (r = -r));
//     h = (r >> 2);
//     h = (h << 1) + h;

//     l = r & 3;
//     l = (l << 1) + l;
//     l = l >> 2;

//     r = h + l;
//     (s == 1 && (r = -r));
//     ((x == mask) && (r = 0xa0000000));

//     return r;
// }

// 0 <= k < w-1
int threefourths_2(int x)
{
    long long int y = x;

    return y * 3 / 4;
}


void test_threefourths(int x)
{
    if (threefourths(x) != threefourths_2(x)) {
        printf("u=0x%08x, threefourths:%08x, threefourths_2:%08x\n",
                x,
                threefourths(x),
                threefourths_2(x));
        abort();
    }
}

// random test
void test()
{
    int seed;
    unsigned r;

    assert(threefourths(0) == 0);
    assert(threefourths(1) == 0);
    assert(threefourths(4) == 3);
    assert(threefourths(3) == 2);
    assert(threefourths(-3) == -2);
    assert(threefourths(-4) == -3);

    // test INT_MIN
    for (int i = INT_MIN; i < INT_MIN + 100; i++)
        test_threefourths(i);

    // test zero
    for (int i = -256; i <= 256; i++)
        test_threefourths(i);

    // test INT_MAX
    for (int i = INT_MAX - 100; i <= INT_MAX; i++) {
        test_threefourths(i);

        if (i == INT_MAX)
            break;
    }

    // test random
    seed = time(NULL);
    srandom(seed);
    for (int i = 0; i < 1000000; i++) {
        // 随机数的大小是0-2147483647
        r = (random() << 1) - 1;
        test_threefourths(r);
    }
}


int main()
{
    test();
}