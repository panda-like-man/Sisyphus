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


unsigned float2unsigned(float f) {
    number n;

    n.f = f;

    return n.u;
}

float unsigned2float(unsigned u) {
    number n;

    n.u = u;

    return n.f;
}

// 计算-f，如果f是NaN，则返回f
// 注意inf和nan的表示
// exp全1，frac全0代表inf
// exp全1，frac不为0代表nan
float_bits float_negate(float_bits f) {
    unsigned sign = f >> 31;
    unsigned exp = f >> 23 & 0xff;
    unsigned frac = f & 0x7fffff;

    if (exp == 0xff && frac)
        return f;

    sign = ~sign;

    return (sign << 31) | (exp << 23) | frac;
}

void test()
{
    unsigned i;
    number n;

    for (i = 0; i <= UINT_MAX; i++) {
        n.u = i;
        if (!isnan(n.f))
            n.f = -n.f;

        if (float_negate(i) != n.u) {
            printf("x=%08x, -x=%f, float_negate:%08x\n",
                i,
                n.f,
                float_negate(i));
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