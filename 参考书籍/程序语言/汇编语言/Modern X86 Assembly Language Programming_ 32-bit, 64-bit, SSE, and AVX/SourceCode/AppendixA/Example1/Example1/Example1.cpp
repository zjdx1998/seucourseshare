#include "stdafx.h"

extern "C" int CalcResult1_(int a, int b, int c);

int _tmain(int argc, _TCHAR* argv[])
{
    int a = 30;
    int b = 20;
    int c = 10;
    int d = CalcResult1_(a, b, c);

    printf("a: %4d  b: %4d  c: %4d\n", a, b, c);
    printf("d: %4d\n", d);
    return 0;
}
