#include <stdio.h>
#include <assert.h>
#include <limits.h>


// for test
int csub_ok(char x, char y)
{
    char diff = x - y;
    int cond1 = (x < 0) && (y > 0) && (diff > 0);
    int cond2 = (x >= 0) && (y < 0) && (diff < 0);

    return cond1 == 0 && cond2 == 0;
}


// 相减无溢出，则返回1
// 同号相减，则无溢出
// 注意考虑x=0，y=Tmin时的情况
// 此外tsub_ok不能由tadd_ok(x, -y)表示，当y=Tmin时，这种情况下会判断错误
int tsub_ok(int x, int y)
{
    int diff = x - y;
    int cond1 = (x < 0) && (y > 0) && (diff > 0);
    int cond2 = (x >= 0) && (y < 0) && (diff < 0);

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
            char diff = i-j;
            if (diff != x-y) {
                printf("overflow: i = %d, j = %d, diff = %d\n", i, j, diff);
                assert(csub_ok(i, j) == 0);
            } else {
                printf("sub ok: i = %d, j = %d, diff = %d\n", i, j , diff);
                assert(csub_ok(i, j) == 1);
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
