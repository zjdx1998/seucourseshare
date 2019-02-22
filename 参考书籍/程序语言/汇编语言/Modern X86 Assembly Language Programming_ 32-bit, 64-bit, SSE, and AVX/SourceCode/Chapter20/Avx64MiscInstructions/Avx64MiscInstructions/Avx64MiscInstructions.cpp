#include "stdafx.h"
#include "MiscDefs.h"
#define _USE_MATH_DEFINES
#include <math.h>

extern "C" void Avx64GatherFloatIndx32_(float g[8], const float* x, Int32 indices[8]);
extern "C" void Avx64GatherFloatIndx64_(float g[4], const float* x, Int64 indices[4]);
extern "C" void Avx64FloatToHp_(Uint16 x_hp[8], float x1[8]);
extern "C" void Avx64HpToFloat_(float x[8], Uint16 x_hp[8]);

void Avx64GatherFloat(void)
{
    const int n = 20;
    float x1[n];

    printf("Results for Avx64GatherFloat()\n");
    printf("\nSource array\n");

    for (int i = 0; i < n; i++)
    {
        x1[i] = i * 100.0f;
        printf("x1[%02d]: %6.1f\n", i, x1[i]);
    }
    printf("\n");

    float g1_32[8], g1_64[4];
    Int32 g1_indices32[8] = {2, 3, 7, 1, 1, 12, 4, 17};
    Int64 g1_indices64[4] = {5, 0, 19, 13};

    Avx64GatherFloatIndx32_(g1_32, x1, g1_indices32);
    for (int i = 0; i < 8; i++)
        printf("g1_32[%02d] = %6.1f (gathered from x[%02d])\n", i, g1_32[i], g1_indices32[i]);

    printf("\n");

    Avx64GatherFloatIndx64_(g1_64, x1, g1_indices64);
    for (int i = 0; i < 4; i++)
        printf("g1_64[%02d] = %6.1f (gathered from x[%02lld])\n", i, g1_64[i], g1_indices64[i]);
}

void Avx64HalfPrecision(void)
{
    float x1[8], x2[8];
    Uint16 x_hp[8];

    x1[0] = 0.5f;            x1[1] = 1.0f / 512.0f;
    x1[2] = 1004.0625f;      x1[3] = 5003.125f;
    x1[4] = 42000.5f;        x1[5] = 75600.875f;
    x1[6] = -6002.125f;      x1[7] = (float)M_PI;

    Avx64FloatToHp_(x_hp, x1);
    Avx64HpToFloat_(x2, x_hp);

    printf("\nResults for Avx64HalfPrecision()\n");

    for (int i = 0; i < 8; i++)
        printf("%d %16.6f %16.6f\n", i, x1[i], x2[i]);
}

int _tmain(int argc, _TCHAR* argv[])
{
    Avx64GatherFloat();
    Avx64HalfPrecision();
    return 0;
}
