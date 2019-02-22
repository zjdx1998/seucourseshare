#include "stdafx.h"
#include "YmmVal.h"

extern "C" void AvxPiUnpackDQ_(YmmVal* a, YmmVal* b, YmmVal c[2]);
extern "C" void AvxPiPackDW_(YmmVal* a, YmmVal* b, YmmVal* c);

void AvxPiUnpackDQ(void)
{
    __declspec(align(32))  YmmVal a;
    __declspec(align(32))  YmmVal b;
    __declspec(align(32))  YmmVal c[2];

    a.i32[0] = 0x0000;  b.i32[0] = 0x8888;
    a.i32[1] = 0x1111;  b.i32[1] = 0x9999;
    a.i32[2] = 0x2222;  b.i32[2] = 0xaaaa;
    a.i32[3] = 0x3333;  b.i32[3] = 0xbbbb;

    a.i32[4] = 0x4444;  b.i32[4] = 0xcccc;
    a.i32[5] = 0x5555;  b.i32[5] = 0xdddd;
    a.i32[6] = 0x6666;  b.i32[6] = 0xeeee;
    a.i32[7] = 0x7777;  b.i32[7] = 0xffff;

    AvxPiUnpackDQ_(&a, &b, c);

    printf("\nResults for AvxPiUnpackDQ()\n\n");
    printf("i   a           b           vpunpckldq  vpunpckhdq\n");
    printf("--------------------------------------------------\n");

    for (int i = 0; i < 8; i++)
    {
        const char* fs = "0x%04X      ";

        printf("%-2d  ", i);
        printf(fs, a.u32[i]);
        printf(fs, b.u32[i]);
        printf(fs, c[0].u32[i]);
        printf(fs, c[1].u32[i]);
        printf("\n");
    }
}

void AvxPiPackDW(void)
{
    char buff[256];
    __declspec(align(32))  YmmVal a;
    __declspec(align(32))  YmmVal b;
    __declspec(align(32))  YmmVal c;

    a.i32[0] = 10;          b.i32[0] = 32768;
    a.i32[1] = -200000;     b.i32[1] = 6500;
    a.i32[2] = 300000;      b.i32[2] = 42000;
    a.i32[3] = -4000;       b.i32[3] = -68000;

    a.i32[4] = 9000;        b.i32[4] = 25000;
    a.i32[5] = 80000;       b.i32[5] = 500000;
    a.i32[6] = 200;         b.i32[6] = -7000;
    a.i32[7] = -32769;      b.i32[7] = 12500;

    AvxPiPackDW_(&a, &b, &c);
    printf("\nResults for AvxPiPackDW()\n\n");

    printf("a lo %s\n", a.ToString_i32(buff, sizeof(buff), false));
    printf("a hi %s\n", a.ToString_i32(buff, sizeof(buff), true));
    printf("\n");

    printf("b lo %s\n", b.ToString_i32(buff, sizeof(buff), false));
    printf("b hi %s\n", b.ToString_i32(buff, sizeof(buff), true));
    printf("\n");

    printf("c lo %s\n", c.ToString_i16(buff, sizeof(buff), false));
    printf("c hi %s\n", c.ToString_i16(buff, sizeof(buff), true));
}

int _tmain(int argc, _TCHAR* argv[])
{
    AvxPiUnpackDQ();
    AvxPiPackDW();
    return 0;
}
