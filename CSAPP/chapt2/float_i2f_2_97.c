#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>

typedef unsigned float_bits;

typedef union __number
{
    unsigned   u;
    float      f;
}number;


int get_most_bit(unsigned i) {
    int j = 0;

    i = i >> 1;
    while (i) {
        j++;
        i = i >> 1;
    }

    return j;
}

// compute (float)i
float_bits float_i2f(int i)
{
    unsigned sign = 0;
    unsigned exp;
    unsigned frac;
    unsigned mask;

    // case 1: INT_MIN
    if (i == INT_MIN) {
        sign = 1;
        exp = 127 + 31;
        frac = 0;

        return (sign << 31) | (exp << 23) | frac;
    }

    // case 2: 0
    if (i == 0)
        return 0;

    if (i < 0) {
        sign = 1;
        i = -i;
    }

    // case 3: less than 2**24
    if (i < (1 << 24)) {
        exp = get_most_bit(i);

        frac = i - (1 << exp);
        frac = frac << (23-exp);
        exp += 127;

        return (sign << 31) | (exp << 23) | frac;
    }

    // case 4: other, need round, 画图去理解下吧
    exp = get_most_bit(i);
    mask = 1 << (exp - 24);
    // plus
    if (i & (mask - 1) && (i & mask))
        i += mask;
    // to even
    else if ((i & mask) && (i & (mask << 1)))
        i += mask;

    exp = get_most_bit(i);
    frac = i - (1 << exp);
    frac = frac >> (exp - 23);
    exp += 127;

    return (sign << 31) | (exp << 23) | frac;
}


void test()
{
    int i;
    number n;
    for (i = INT_MIN; i <= INT_MAX; i++) {
        n.f = (float)i;
        if (n.u != float_i2f(i)) {
            printf("x:%08x, answer:%08x, cur:%08x\n",
                i,
                n.u,
                float_i2f(i));
            abort();
        }

        if (i == INT_MAX)
            break;
        // if ((unsigned)i % 1000000 == 1)
        //     printf("test 1000000: %d\n", i);
    }
}

int main()
{
    test();
}