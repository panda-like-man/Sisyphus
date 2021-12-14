#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

// 改编自2
int saturating_add(int x, int y)
{
    int sum = x + y;
    int mask = INT_MIN;

    // postive !x,  negtive: x
    int xs = x & mask;
    int ys = y & mask;
    int sums = sum & mask;

    int cond1 = !!x && !xs && !!y && !ys && sums;
    int cond2 = !!x && xs && !!y && ys && !sums;

    (cond1 && (sum = INT_MAX)) || (cond2 && (sum = mask));

    return sum;
}


int saturating_add_2(int x, int y)
{
    int sum = x + y;
    int cond1 = (x > 0) && (y > 0) && (sum < 0);
    int cond2 = (x < 0) && (y < 0) && (sum >= 0);

    if      (cond1) return INT_MAX;
    else if (cond2) return INT_MIN;
    else            return sum;
}


void test_saturating_add(int x, int y)
{
    if (saturating_add(x, y) != saturating_add_2(x, y)) {
        printf("x=0x%08x, y=0x%08x, saturating:0x%08x, saturating_2:0x%08x\n",
                x,
                y,
                saturating_add(x, y),
                saturating_add_2(x, y));
        abort();
    }
}

// random test
void test()
{
    int seed;
    unsigned r;

    assert(saturating_add(INT_MAX, 1) == INT_MAX);
    assert(saturating_add(0x80000000, 0x80000000) == 0x80000000);
    assert(saturating_add(0, 0) == 0);
    assert(saturating_add(-1, -1) == -2);
    assert(saturating_add(1, 1) == 2);
    assert(saturating_add(1, -1) == 0);

    // test random
    seed = time(NULL);
    srandom(seed);

    // test INT_MIN
    for (int i = INT_MIN; i < INT_MIN + 100; i++) {
        r = (random() << 1) - 1;
        test_saturating_add(i, r);
    }
    // test zero
    for (int i = -256; i <= 256; i++) {
        r = (random() << 1) - 1;
        test_saturating_add(i, r);
    }

    // test INT_MAX
    for (int i = INT_MAX - 100; i <= INT_MAX; i++) {
        r = (random() << 1) - 1;
        test_saturating_add(i, r);
        if (i == INT_MAX)
            break;
    }

    for (int i = 0; i < 1000000; i++) {
        r = (random() << 1) - 1;
        int j = (random() << 1) - 1;

        test_saturating_add(r, j);
    }
}


int main()
{
    test();
}