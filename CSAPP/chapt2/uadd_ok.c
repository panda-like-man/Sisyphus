#include <stdio.h>
#include <assert.h>
#include <limits.h>

// 相加无溢出，则返回1
// 1. 无符号相加正常时，和肯定大于等于x或者y
// 2. 注意考虑x或者y为0的情况，如y=0，此时x=x+y
int uadd_ok(unsigned x, unsigned y)
{
    return x <= x+y;
}


