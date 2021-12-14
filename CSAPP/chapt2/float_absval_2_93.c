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

float_bits float_absval(float_bits f) {
    unsigned sign = f >> 31;
    unsigned exp = f >> 23 & 0xff;
    unsigned frac = f & 0x7fffff;

    if (exp == 0xff && frac)
        return f;

    sign = 0;

    return (sign << 31) | (exp << 23) | frac;
}

void test()
{
    unsigned i;
    number n;

    for (i = 0; i <= UINT_MAX; i++) {
        n.u = i;
        if (!isnan(n.f) && n.f < 0)
            n.f = -n.f;
        if (n.u == 0x80000000)
            n.u = 0;

        if (float_absval(i) != n.u) {
            printf("x=%08x, |x|=%f, float_absval:%08x\n",
                i,
                n.f,
                float_absval(i));
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