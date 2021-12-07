#include <stdio.h>
#include <assert.h>
#include <limits.h>


// for test
int cadd_ok(char x, char y)
{
    char sum = x + y;
    int cond1 = (x > 0) && (y > 0) && (sum < 0);
    int cond2 = (x < 0) && (y < 0) && (sum >= 0);

    return cond1 == 0 && cond2 == 0;
}


// 相加无溢出，则返回1
// 符号相异，则不溢出
// 注意考虑Tmin的情况，两个Tmin相加，结果为0
int tadd_ok(int x, int y)
{
    int sum = x + y;
    int cond1 = (x > 0) && (y > 0) && (sum < 0);
    int cond2 = (x < 0) && (y < 0) && (sum >= 0);

    return cond1 == 0 && cond2 == 0;
}


// 这里测试时，为了避免测试时间较长，使用的是char型的数据
void test()
{
    int x, y;

    for (char i = CHAR_MIN; i <= CHAR_MAX; i++)
    {
        for (char j = CHAR_MIN; j <= CHAR_MAX; j++)
        {
            x = i;
            y = j;
            char sum = i+j;
            if (sum != x+y) {
                printf("overflow: i = %d, j = %d, sum = %d\n", i, j, sum);
                assert(cadd_ok(i, j) == 0);
            } else {
                printf("add ok: i = %d, j = %d, sum = %d\n", i, j, sum);
                assert(cadd_ok(i, j) == 1);
            }
            if (j == CHAR_MAX)
                break;
        }
        if (x == CHAR_MAX)
            break;
    }

    printf("ok\n");
}


int main()
{
    test();
}