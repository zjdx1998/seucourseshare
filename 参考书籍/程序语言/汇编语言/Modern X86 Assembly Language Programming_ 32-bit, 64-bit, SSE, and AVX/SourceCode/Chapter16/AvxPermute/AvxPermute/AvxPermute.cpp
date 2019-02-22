#include "stdafx.h"
#include "YmmVal.h"
#include <math.h>

extern "C" void AvxPermuteInt32_(YmmVal* des, YmmVal* src, YmmVal* ind);
extern "C" void AvxPermuteFloat_(YmmVal* des, YmmVal* src, YmmVal* ind);
extern "C" void AvxPermuteFloatIl_(YmmVal* des, YmmVal* src, YmmVal* ind);

void AvxPermuteInt32(void)
{
    __declspec(align(32)) YmmVal des, src, ind;

    src.i32[0] = 10;        ind.i32[0] = 3;
    src.i32[1] = 20;        ind.i32[1] = 7;
    src.i32[2] = 30;        ind.i32[2] = 0;
    src.i32[3] = 40;        ind.i32[3] = 4;
    src.i32[4] = 50;        ind.i32[4] = 6;
    src.i32[5] = 60;        ind.i32[5] = 6;
    src.i32[6] = 70;        ind.i32[6] = 1;
    src.i32[7] = 80;        ind.i32[7] = 2;

    AvxPermuteInt32_(&des, &src, &ind);

    printf("\nResults for AvxPermuteInt32()\n");
    for (int i = 0; i < 8; i++)
    {
        printf("des[%d]: %5d  ", i, des.i32[i]);
        printf("ind[%d]: %5d  ", i, ind.i32[i]);
        printf("src[%d]: %5d  ", i, src.i32[i]);
        printf("\n");
    }
}

void AvxPermuteFloat(void)
{
    __declspec(align(32)) YmmVal des, src, ind;

    // src1 indices must be between 0 and 7.
    src.r32[0] = 800.0f;       ind.i32[0] = 3;
    src.r32[1] = 700.0f;       ind.i32[1] = 7;
    src.r32[2] = 600.0f;       ind.i32[2] = 0;
    src.r32[3] = 500.0f;       ind.i32[3] = 4;
    src.r32[4] = 400.0f;       ind.i32[4] = 6;
    src.r32[5] = 300.0f;       ind.i32[5] = 6;
    src.r32[6] = 200.0f;       ind.i32[6] = 1;
    src.r32[7] = 100.0f;       ind.i32[7] = 2;

    AvxPermuteFloat_(&des, &src, &ind);

    printf("\nResults for AvxPermuteFloat()\n");
    for (int i = 0; i < 8; i++)
    {
        printf("des[%d]: %8.1f  ", i, des.r32[i]);
        printf("ind[%d]: %5d  ", i, ind.i32[i]);
        printf("src[%d]: %8.1f  ", i, src.r32[i]);
        printf("\n");
    }
}

void AvxPermuteFloatIl(void)
{
    __declspec(align(32)) YmmVal des, src, ind;

    // Lower lane
    src.r32[0] = sqrt(10.0f);       ind.i32[0] = 3;
    src.r32[1] = sqrt(20.0f);       ind.i32[1] = 2;
    src.r32[2] = sqrt(30.0f);       ind.i32[2] = 2;
    src.r32[3] = sqrt(40.0f);       ind.i32[3] = 0;

    // Upper lane
    src.r32[4] = sqrt(50.0f);       ind.i32[4] = 1;
    src.r32[5] = sqrt(60.0f);       ind.i32[5] = 3;
    src.r32[6] = sqrt(70.0f);       ind.i32[6] = 3;
    src.r32[7] = sqrt(80.0f);       ind.i32[7] = 2;

    AvxPermuteFloatIl_(&des, &src, &ind);

    printf("\nResults for AvxPermuteFloatIl()\n");
    for (int i = 0; i < 8; i++)
    {
        if (i == 0)
            printf("Lower lane\n");
        else if (i == 4)
            printf("Upper lane\n");

        printf("des[%d]: %8.4f  ", i, des.r32[i]);
        printf("ind[%d]: %5d  ", i, ind.i32[i]);
        printf("src[%d]: %8.4f  ", i, src.r32[i]);
        printf("\n");
    }
}

int _tmain(int argc, _TCHAR* argv[])
{
    AvxPermuteInt32();
    AvxPermuteFloat();
    AvxPermuteFloatIl();
    return 0;
}
