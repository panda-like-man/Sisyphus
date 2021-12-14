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
float_bits float_half(float_bits f) {
    unsigned sign = f >> 31;
    unsigned exp = f >> 23 & 0xff;
    unsigned frac = f & 0x7fffff;

    // case 1: nan and inf
    if (exp == 0xff)
        return f;

    // case 2: norms
    if (exp >= 1 && exp <= 254) {
        if (exp > 1)
            exp -= 1;
        else {
            frac = (1 << 23) + frac;
            if ((frac & 3) == 3)
                frac += 1;

            exp = 0;
            frac >>= 1;
        }

        return (sign << 31) | (exp << 23) | frac;
    }

    // case 3: 0
    if (f == 0)
        return 0;

    // case 4: denorms
    // need round?
    if ((frac & 3) == 3)
        frac += 1;

    frac >>= 1;

    return (sign << 31) | (exp << 23) | frac;
}

void test()
{
    unsigned i;
    number n;

    for (i = 0; i <= UINT_MAX; i++) {
        n.u = i;
        if (!isnan(n.f))
            n.f = n.f * 0.5;

        if (float_half(i) != n.u) {
            printf("x=%08x, x*0.5=%08x, float_half:%08x\n",
                i,
                n.u,
                float_half(i));
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