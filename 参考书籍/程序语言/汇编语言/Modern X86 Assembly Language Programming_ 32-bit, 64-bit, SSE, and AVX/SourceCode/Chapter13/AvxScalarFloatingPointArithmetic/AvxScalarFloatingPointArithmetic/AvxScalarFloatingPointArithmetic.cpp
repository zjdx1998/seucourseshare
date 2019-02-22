#include "stdafx.h"

extern "C" void AvxSfpArithmetic_(double a, double b, double results[8]);

int _tmain(int argc, _TCHAR* argv[])
{
    const int n = 8;
    const char* inames[n] =
    {
        "vaddsd", "vsubsd", "vmulsd", "vdivsd",
        "vminsd", "vmaxsd", "vsqrtsd a", "fabs b"
    };

    double a = 17.75;
    double b = -39.1875;
    double c[n];

    AvxSfpArithmetic_(a, b, c);

    printf("\nResults for AvxScalarFloatingPointArithmetic\n");
    printf("a:              %.6lf\n", a);
    printf("b:              %.6lf\n", b);
    for (int i = 0; i < n; i++)
        printf("%-14s  %-12.6lf\n", inames[i], c[i]);

    return 0;
}
