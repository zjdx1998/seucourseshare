#include "stdafx.h"
#include "YmmVal.h"

extern "C" void AvxPfpArithmeticFloat_(const YmmVal* a, const YmmVal* b, YmmVal c[6]);
extern "C" void AvxPfpArithmeticDouble_(const YmmVal* a, const YmmVal* b, YmmVal c[5]);

void AvxPfpArithmeticFloat(void)
{
    __declspec(align(32)) YmmVal a;
    __declspec(align(32)) YmmVal b;
    __declspec(align(32)) YmmVal c[6];

    a.r32[0] = 2.0f;      b.r32[0] = 12.5f;
    a.r32[1] = 3.5f;      b.r32[1] = 52.125f;
    a.r32[2] = -10.75f;   b.r32[2] = 17.5f;
    a.r32[3] = 15.0f;     b.r32[3] = 13.982f;
    a.r32[4] = -12.125f;  b.r32[4] = -4.75f;
    a.r32[5] = 3.875f;    b.r32[5] = 3.0625f;
    a.r32[6] = 2.0f;      b.r32[6] = 7.875f;
    a.r32[7] = -6.35f;    b.r32[7] = -48.1875f;

    AvxPfpArithmeticFloat_(&a, &b, c);

    printf("Results for AvxPfpArithmeticFloat()\n\n");

    printf(" i        a        b      Add      Sub      Mul      Div      Abs      Neg\n");
    printf("--------------------------------------------------------------------------\n");

    for (int i = 0; i < 8; i++)
    {
        const char* fs = "%8.3f ";

        printf("%2d ", i);
        printf(fs, a.r32[i]);
        printf(fs, b.r32[i]);
        printf(fs, c[0].r32[i]);
        printf(fs, c[1].r32[i]);
        printf(fs, c[2].r32[i]);
        printf(fs, c[3].r32[i]);
        printf(fs, c[4].r32[i]);
        printf(fs, c[5].r32[i]);
        printf("\n");
    }
}

void AvxPfpArithmeticDouble(void)
{
    __declspec(align(32)) YmmVal a;
    __declspec(align(32)) YmmVal b;
    __declspec(align(32)) YmmVal c[5];

    a.r64[0] = 12.0;     b.r64[0] = 0.875;
    a.r64[1] = 13.5;     b.r64[1] = -125.25;
    a.r64[2] = 18.75;    b.r64[2] = 72.5;
    a.r64[3] = 5.0;      b.r64[3] = -98.375;

    AvxPfpArithmeticDouble_(&a, &b, c);

    printf("\n\nResults for AvxPfpArithmeticDouble()\n\n");

    printf(" i        a          b       Min       Max    Sqrt a    HorAdd    HorSub\n");
    printf("--------------------------------------------------------------------------\n");

    for (int i = 0; i < 4; i++)
    {
        const char* fs = "%9.3lf ";

        printf("%2d ", i);
        printf(fs, a.r64[i]);
        printf(fs, b.r64[i]);
        printf(fs, c[0].r64[i]);
        printf(fs, c[1].r64[i]);
        printf(fs, c[2].r64[i]);
        printf(fs, c[3].r64[i]);
        printf(fs, c[4].r64[i]);
        printf("\n");
    }
}

int _tmain(int argc, _TCHAR* argv[])
{
    AvxPfpArithmeticFloat();
    AvxPfpArithmeticDouble();
    return 0;
}
