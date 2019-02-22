#include "stdafx.h"

extern "C" void CalcResult2_(int a, int b, int c, int* quo, int* rem);

int _tmain(int argc, _TCHAR* argv[])
{
    int a = 75;
    int b = 125;
    int c = 7;
    int quo, rem;

    CalcResult2_(a, b, c, &quo, &rem);

    printf("a:   %4d  b:   %4d  c: %4d\n", a, b, c);
    printf("quo: %4d  rem: %4d\n", quo, rem);
    return 0;
}
