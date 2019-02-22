#include "stdafx.h"
#include "XmmVal.h"

extern "C" void SsePiAddI16_(const XmmVal* a, const XmmVal* b, XmmVal c[2]);
extern "C" void SsePiSubI32_(const XmmVal* a, const XmmVal* b, XmmVal* c);
extern "C" void SsePiMul32_(const XmmVal* a, const XmmVal* b, XmmVal c[2]);

void SsePiAddI16(void)
{
    _declspec(align(16)) XmmVal a;
    _declspec(align(16)) XmmVal b;
    _declspec(align(16)) XmmVal c[2];
    char buff[256];

    a.i16[0] = 10;          b.i16[0] = 100;
    a.i16[1] = 200;         b.i16[1] = -200;
    a.i16[2] = 30;          b.i16[2] = 32760;
    a.i16[3] = -32766;      b.i16[3] = -400;
    a.i16[4] = 50;          b.i16[4] = 500;
    a.i16[5] = 60;          b.i16[5] = -600;
    a.i16[6] = 32000;       b.i16[6] = 1200;
    a.i16[7] = -32000;      b.i16[7] = -950;

    SsePiAddI16_(&a, &b, c);

    printf("\nResults for SsePiAddI16_\n");
    printf("a:    %s\n", a.ToString_i16(buff, sizeof(buff)));
    printf("b:    %s\n", b.ToString_i16(buff, sizeof(buff)));
    printf("c[0]: %s\n", c[0].ToString_i16(buff, sizeof(buff)));
    printf("\n");
    printf("a:    %s\n", a.ToString_i16(buff, sizeof(buff)));
    printf("b:    %s\n", b.ToString_i16(buff, sizeof(buff)));
    printf("c[1]: %s\n", c[1].ToString_i16(buff, sizeof(buff)));
}

void SsePiSubI32(void)
{
    _declspec(align(16)) XmmVal a;
    _declspec(align(16)) XmmVal b;
    _declspec(align(8)) XmmVal c;       // Misaligned XmmVal
    char buff[256];

    a.i32[0] = 800;        b.i32[0] = 250;
    a.i32[1] = 500;        b.i32[1] = -2000;
    a.i32[2] = 1000;       b.i32[2] = -40;
    a.i32[3] = 900;        b.i32[3] = 1200;

    SsePiSubI32_(&a, &b, &c);

    printf("\nResults for SsePiSubI32_\n");
    printf("a: %s\n", a.ToString_i32(buff, sizeof(buff)));
    printf("b: %s\n", b.ToString_i32(buff, sizeof(buff)));
    printf("c: %s\n", c.ToString_i32(buff, sizeof(buff)));
}

void SsePiMul32(void)
{
    __declspec(align(16)) XmmVal a;
    __declspec(align(16)) XmmVal b;
    __declspec(align(16)) XmmVal c[2];
    char buff[256];

    a.i32[0] = 10;          b.i32[0] = 100;
    a.i32[1] = 20;          b.i32[1] = -200;
    a.i32[2] = -30;         b.i32[2] = 300;
    a.i32[3] = -40;         b.i32[3] = -400;

    SsePiMul32_(&a, &b, c);

    printf("\nResults for SsePiMul32_\n");
    printf("a:    %s\n", a.ToString_i32(buff, sizeof(buff)));
    printf("b:    %s\n", b.ToString_i32(buff, sizeof(buff)));
    printf("c[0]: %s\n", c[0].ToString_i32(buff, sizeof(buff)));
    printf("\n");
    printf("a:    %s\n", a.ToString_i32(buff, sizeof(buff)));
    printf("b:    %s\n", b.ToString_i32(buff, sizeof(buff)));
    printf("c[1]: %s\n", c[1].ToString_i64(buff, sizeof(buff)));
}

int _tmain(int argc, _TCHAR* argv[])
{
    SsePiAddI16();
    SsePiSubI32();
    SsePiMul32();
    return 0;
}
