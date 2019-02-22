#include "stdafx.h"
#include "YmmVal.h"

extern "C" void AvxPiI16_(YmmVal* a, YmmVal* b, YmmVal c[6]);
extern "C" void AvxPiI32_(YmmVal* a, YmmVal* b, YmmVal c[5]);

void AvxPiI16(void)
{
    __declspec(align(32))  YmmVal a;
    __declspec(align(32))  YmmVal b;
    __declspec(align(32))  YmmVal c[6];

    a.i16[0] = 10;       b.i16[0] = 1000;
    a.i16[1] = 20;       b.i16[1] = 2000;
    a.i16[2] = 3000;     b.i16[2] = 30;
    a.i16[3] = 4000;     b.i16[3] = 40;

    a.i16[4] = 30000;    b.i16[4] = 3000;       // add overflow
    a.i16[5] = 6000;     b.i16[5] = 32000;      // add overflow
    a.i16[6] = 2000;     b.i16[6] = -31000;     // sub overflow
    a.i16[7] = 4000;     b.i16[7] = -30000;     // sub overflow

    a.i16[8] = 4000;    b.i16[8] = -2500;
    a.i16[9] = 3600;     b.i16[9] = -1200;
    a.i16[10] = 6000;    b.i16[10] = 9000;
    a.i16[11] = -20000;  b.i16[11] = -20000;

    a.i16[12] = -25000;  b.i16[12] = -27000;    // add overflow
    a.i16[13] = 8000;    b.i16[13] = 28700;     // add overflow
    a.i16[14] = 3;       b.i16[14] = -32766;    // sub overflow
    a.i16[15] = -15000;  b.i16[15] = 24000;     // sub overflow

    AvxPiI16_(&a, &b, c);

    printf("\nResults for AvxPiI16()\n\n");
    printf("i        a       b   vpaddw vpaddsw  vpsubw vpsubsw  vpminsw vpmaxsw\n");
    printf("--------------------------------------------------------------------\n");

    for (int i = 0; i < 16; i++)
    {
        const char* fs = "%7d ";

        printf("%2d ", i);
        printf(fs, a.i16[i]);
        printf(fs, b.i16[i]);
        printf(fs, c[0].i16[i]);
        printf(fs, c[1].i16[i]);
        printf(fs, c[2].i16[i]);
        printf(fs, c[3].i16[i]);
        printf(fs, c[4].i16[i]);
        printf(fs, c[5].i16[i]);
        printf("\n");
    }
}

void AvxPiI32(void)
{
    __declspec(align(32))  YmmVal a;
    __declspec(align(32))  YmmVal b;
    __declspec(align(32))  YmmVal c[5];

    a.i32[0] = 64;        b.i32[0] = 4;
    a.i32[1] = 1024;      b.i32[1] = 5;
    a.i32[2] = -2048;     b.i32[2] = 2;
    a.i32[3] = 8192;      b.i32[3] = 5;
    a.i32[4] = -256;      b.i32[4] = 8;
    a.i32[5] = 4096;      b.i32[5] = 7;
    a.i32[6] = 16;        b.i32[6] = 3;
    a.i32[7] = 512;       b.i32[7] = 6;

    AvxPiI32_(&a, &b, c);

    printf("\nResults for AvxPiI32()\n\n");
    printf("i         a        b    vphaddd  vphsubd  vpmulld  vpsllvd  vpsravd\n");
    printf("-------------------------------------------------------------------\n");

    for (int i = 0; i < 8; i++)
    {
        const char* fs = "%8d ";

        printf("%2d ", i);
        printf(fs, a.i32[i]);
        printf(fs, b.i32[i]);
        printf(fs, c[0].i32[i]);
        printf(fs, c[1].i32[i]);
        printf(fs, c[2].i32[i]);
        printf(fs, c[3].i32[i]);
        printf(fs, c[4].i32[i]);
        printf("\n");
    }
}

int _tmain(int argc, _TCHAR* argv[])
{
    AvxPiI16();
    AvxPiI32();
    return 0;
}
