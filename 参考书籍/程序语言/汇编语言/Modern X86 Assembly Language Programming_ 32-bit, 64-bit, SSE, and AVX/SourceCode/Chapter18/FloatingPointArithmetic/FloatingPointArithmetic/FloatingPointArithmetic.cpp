#include "stdafx.h"

extern "C" double CalcSum_(float a, double b, float c, double d, float e, double f);
extern "C" double CalcDist_(int x1, double x2, long long y1, double y2, float z1, short z2);

void CalcSum(void)
{
    float a = 10.0f;
    double b = 20.0;
    float c = 0.5f;
    double d = 0.0625;
    float e = 15.0f;
    double f = 0.125;

    double sum = CalcSum_(a, b, c, d, e, f);

    printf("\nResults for CalcSum()\n");
    printf("a: %10.4f  b: %10.4lf c: %10.4f\n", a, b, c);
    printf("d: %10.4lf  e: %10.4f f: %10.4lf\n", d, e, f);
    printf("\nsum: %10.4lf\n", sum);
}

void CalcDist(void)
{
    int x1 = 5;
    double x2 = 12.875;
    long long y1 = 17;
    double y2 = 23.1875;
    float z1 = -2.0625;
    short z2 = -6;

    double dist = CalcDist_(x1, x2, y1, y2, z1, z2);

    printf("\nResults for CalcDist()\n");
    printf("x1: %10d  x2: %10.4lf\n", x1, x2);
    printf("y1: %10lld  y2: %10.4lf\n", y1, y2);
    printf("z1: %10.4f  z2: %10d\n", z1, z2);
    printf("\ndist: %12.6lf\n", dist);
}

int _tmain(int argc, _TCHAR* argv[])
{
    CalcSum();
    CalcDist();
    return 0;
}
