#include "stdafx.h"

extern "C" double CalcResult3_(__int64 a, __int64 b, double c, double d);

int _tmain(int argc, _TCHAR* argv[])
{
    __int64 a = 10;
    __int64 b = -15;
    double c = 2.0;
    double d = -3.0;

    double e = CalcResult3_(a, b, c, d);

    printf("a: %lld  b: %lld  c: %.4lf  d: %.4lf\n", a, b, c, d);
    printf("e: %.4lf\n", e);
    return 0;
}
