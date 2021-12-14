#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

/*
    注意fits_bits_2的函数实现
    如果没显式类型转换，可能和预期不符

    // 1 << 31导致的问题
    int main(int argc, char *argv[])
    {
        int x = atoi(argv[1]);
        int n = atoi(argv[2]);
        //int q = x <= ((1 << n) - 1);
        int p = x <= ((1 << 31) - 1);
        return p;
    }
    上面这段代码，在n=31时会有问题
    例如x=0，n=31时
    p = 1但是 q = 0
    编译器做比较然后赋值时，p和q那一行的汇编代码不同
    p那一行编译器会做优化，认为是总成功的，所以是1
    q那一行编译器生成的汇编代码如下
        sall	%cl, %edx
        cmpl	%ebp, %edx
        setg	%al
        movzbl	%al, %eax
    注意位移操作后，编译器做比较时用的setg
    写成对应的c代码大概是
    (1<<31) > x
    这里我猜编译器可能是想避免有符号数到无符号数的转换
    但是生成的这段代码和最初的意图有点违背了
    最初就是想隐式转换的

    这里也告诉我们，不要依赖隐式转换，它可能和预期不一致
    代码显式的转换一下，意思会更明确，也不容易出错
    int q = (unsigned)x <= ((1 << n) - 1);
    对应的汇编如下
        sall	%cl, %edx
        subl	$1, %edx
        cmpl	%ebp, %edx
        setnb	%al
        movzbl	%al, %eax

    可以看到由setg(有符号)变为了setnb(无符号)比较
*/


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