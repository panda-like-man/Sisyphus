#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>

typedef unsigned float_bits;

typedef union __number
{
    unsigned int u;
    float        f;
}number;

int float2int(float f)
{
    if (isnan(f) || f > INT_MAX || f < INT_MIN)
        return 0x80000000;

    return (int)f;
}


// compute (int)f
// if overflow or f is nan, return 0x80000000
int float_f2i(float_bits f)
{
    unsigned sign = f >> 31;
    unsigned exp = f >> 23 & 0xff;
    unsigned frac = f & 0x7fffff;
    int result;

    // case 1: f == +0 or -0
    if (f == 0 || f == 0x80000000)
        return 0;

    // case 2: |f| < 1
    if (exp < 127)
        return 0;

    // case 3: 2**23
    if (exp <= 127 + 23) {
        exp = exp - 127;
        result = (1 << exp) | (frac >> (23 - exp));

        if (sign)
            result = -result;

        return result;
    }

    // case 4: 2**31 - 1
    if (exp <= 127 + 30) {
        exp = exp -127;
        result = (1 << exp) | (frac << (exp - 23));

        if (sign)
            result = -result;

        return result;
    }

    // case 5 2**31
    if (sign == 1 && (exp == 127 + 31) && (frac == 0)) {
        return 1 << 31;
    }

    // other
    return 1 << 31;
}


void test()
{
    unsigned i;
    number n;
    for (i = 0; i <= UINT_MAX; i++) {
        n.u = i;
        if (float2int(n.f) != float_f2i(i)) {
            printf("x:%08x, answer:%08x, cur:%08x\n",
                i,
                float2int(n.f),
                float_f2i(i));
            abort();
        }

        if (i == UINT_MAX)
            break;
    }
}

int main()
{
    test();
}