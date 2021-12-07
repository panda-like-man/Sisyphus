#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

// 不能使用右移和除法

/*
实现的原理，通过填补或者去除符号位的扩展来实现
注意的地方及技巧
    1. -1的二进制位为全1，可通过适当的左移来获得一个mask: 11...11000...0
    2. 右移的位数是0～31，则对应的mask的左移位数为32～1
    3. 针对第二点不理解时，可通过具体例子来理解，如右移0、1、31位时，-1对应的左移位数
    4. 注意-1左移32位的处理，-1<<32由于移位运算cpu处理时，只处理0-31，大于该值时，
       会取模，则-1<<32相当于没左移，这时我们本来应该得到一个mask为全0，但是由于没有左移，得到
       的为全1，针对这个特殊情况，左移时可通过左移两次来实现，即((-1) << ((w - k - 1)) << 1)
    5. 注意负数右移时才应该补1，正数右移不用补
        那么我们怎么不通过右移操作来得到符号位呢，可以通过下式来实现，好好体会
        !!(x & (1 << w-1))
*/

// 通过算术右移实现逻辑右移
unsigned srl(unsigned x, int k)
{
    unsigned xsra = (int)x >> k;

    int w = sizeof(int) << 3;
    unsigned mask = (-1 << w - k - 1) << 1;
    mask = ~mask;

    return xsra & mask;
}


// 通过逻辑右移实现算术右移
int sra(int x, int k)
{
    int xsrl = (unsigned)x >> k;
    int w = sizeof(int) << 8;

    // 得到符号位
    int s = !!(x & (1 << w-1));
    unsigned mask = (-s << w - k - 1) << 1;

    return xsrl | mask;
}

void test_sr(int r)
{
    int ir;
    unsigned ur;
    for (int j = 0; j < 32; j++) {
        ir = r >> j;
        if (ir != sra(r, j)) {
            printf("sra: r=%d, k=%d, r>>k=%d, sra:%d\n", r, j, ir, sra(r, j));
            abort();
        }

        ur = (unsigned)r >> j;
        if (ur != srl(r, j)) {
            printf("srl: r=%u, k=%d, r>>k=%d, srl:%d\n", r, j, ur, srl(r, j));
            abort();
        }
    }
}

// random test
void test()
{
    int seed, r;

    // test INT_MIN
    for (int i = INT_MIN; i < INT_MIN + 100; i++)
        test_sr(i);

    // test zero
    for (int i = -256; i <= 256; i++)
        test_sr(i);

    // test INT_MAX
    for (int i = INT_MAX - 100; i <= INT_MAX; i++) {
        test_sr(i);
        if (i == INT_MAX)
            break;
    }

    // test random
    seed = time(NULL);
    srandom(seed);
    for (int i = 0; i < 1000000; i++) {
        // 随机数的大小是0-2147483647，所以减去一个bais以保证有正有负
        r = random() - (1 << 30);
        test_sr(r);
    }

}


int main()
{
    // 注意位移操作的位数 i << j 等价  i << (j % 32)
    // for (int i = 0; i < 129; i++)
    //     printf("-1 << %d = %d\n", i, -1 << i);
    test();
}