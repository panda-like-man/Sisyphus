// 觉得麻烦的话，可以直接使用64位long类型做判断
int tmult_ok(int x, int y)
{
    int p = x*y;

    return p==0 || p/x == y;
}

int tmult_ok2(int x, int y)
{
    long long p = (long long)(x) * y;

    return p == (int)p;
}