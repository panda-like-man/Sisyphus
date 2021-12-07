#include <stdio.h>
#include <assert.h>

// 1 <= n <= w
int lower_one_mask(int n)
{
    return ((1 << n - 1) << 1) - 1;
}

int lower_one_mask_2(int n)
{
    return ~((-1 << n - 1) << 1);
}

int main()
{
    assert(lower_one_mask(6) == 0x3f);
    assert(lower_one_mask(17) == 0x1ffff);

    for (int i = 1; i <= 32; i++) {
        assert(lower_one_mask(i) == lower_one_mask_2(i));
    }

    return 0;
}