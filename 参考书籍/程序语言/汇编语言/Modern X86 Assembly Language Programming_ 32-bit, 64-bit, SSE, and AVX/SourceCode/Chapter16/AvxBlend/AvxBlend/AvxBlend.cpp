#include "stdafx.h"
#include "YmmVal.h"

extern "C" void AvxBlendFloat_(YmmVal* des, YmmVal* src1, YmmVal* src2, YmmVal* src3);
extern "C" void AvxBlendByte_(YmmVal* des, YmmVal* src1, YmmVal* src2, YmmVal* src3);

void AvxBlendFloat(void)
{
    char buff[256];
    const Uint32 select1 = 0x00000000;
    const Uint32 select2 = 0x80000000;
    __declspec(align(32)) YmmVal des, src1, src2, src3;

    src1.r32[0] = 100.0f;       src2.r32[0] = -1000.0f;
    src1.r32[1] = 200.0f;       src2.r32[1] = -2000.0f;
    src1.r32[2] = 300.0f;       src2.r32[2] = -3000.0f;
    src1.r32[3] = 400.0f;       src2.r32[3] = -4000.0f;
    src1.r32[4] = 500.0f;       src2.r32[4] = -5000.0f;
    src1.r32[5] = 600.0f;       src2.r32[5] = -6000.0f;
    src1.r32[6] = 700.0f;       src2.r32[6] = -7000.0f;
    src1.r32[7] = 800.0f;       src2.r32[7] = -8000.0f;

    src3.u32[0] = select2;
    src3.u32[1] = select2;
    src3.u32[2] = select1;
    src3.u32[3] = select2;
    src3.u32[4] = select1;
    src3.u32[5] = select1;
    src3.u32[6] = select2;
    src3.u32[7] = select1;

    AvxBlendFloat_(&des, &src1, &src2, &src3);

    printf("\nResults for AvxBlendFloat()\n");
    printf("src1 lo: %s\n", src1.ToString_r32(buff, sizeof(buff), false));
    printf("src1 hi: %s\n", src1.ToString_r32(buff, sizeof(buff), true));
    printf("src2 lo: %s\n", src2.ToString_r32(buff, sizeof(buff), false));
    printf("src2 hi: %s\n", src2.ToString_r32(buff, sizeof(buff), true));
    printf("\n");
    printf("src3 lo: %s\n", src3.ToString_x32(buff, sizeof(buff), false));
    printf("src3 hi: %s\n", src3.ToString_x32(buff, sizeof(buff), true));
    printf("\n");
    printf("des lo:  %s\n", des.ToString_r32(buff, sizeof(buff), false));
    printf("des hi:  %s\n", des.ToString_r32(buff, sizeof(buff), true));
}

void AvxBlendByte(void)
{
    char buff[256];
    __declspec(align(32)) YmmVal des, src1, src2, src3;

    // Control values required to perform doubleword blend
    // using vpblendvb instruction
    const Uint32 select1 = 0x00000000;      // select src1
    const Uint32 select2 = 0x80808080;      // select src2

    src1.i32[0] = 100;          src2.i32[0] = -1000;
    src1.i32[1] = 200;          src2.i32[1] = -2000;
    src1.i32[2] = 300;          src2.i32[2] = -3000;
    src1.i32[3] = 400;          src2.i32[3] = -4000;
    src1.i32[4] = 500;          src2.i32[4] = -5000;
    src1.i32[5] = 600;          src2.i32[5] = -6000;
    src1.i32[6] = 700;          src2.i32[6] = -7000;
    src1.i32[7] = 800;          src2.i32[7] = -8000;

    src3.u32[0] = select1;
    src3.u32[1] = select1;
    src3.u32[2] = select2;
    src3.u32[3] = select1;
    src3.u32[4] = select2;
    src3.u32[5] = select2;
    src3.u32[6] = select1;
    src3.u32[7] = select2;

    AvxBlendByte_(&des, &src1, &src2, &src3);

    printf("\nResults for AvxBlendByte() - doublewords\n");
    printf("src1 lo: %s\n", src1.ToString_i32(buff, sizeof(buff), false));
    printf("src1 hi: %s\n", src1.ToString_i32(buff, sizeof(buff), true));
    printf("src2 lo: %s\n", src2.ToString_i32(buff, sizeof(buff), false));
    printf("src2 hi: %s\n", src2.ToString_i32(buff, sizeof(buff), true));
    printf("\n");
    printf("src3 lo: %s\n", src3.ToString_x32(buff, sizeof(buff), false));
    printf("src3 hi: %s\n", src3.ToString_x32(buff, sizeof(buff), true));
    printf("\n");
    printf("des lo:  %s\n", des.ToString_i32(buff, sizeof(buff), false));
    printf("des hi:  %s\n", des.ToString_i32(buff, sizeof(buff), true));
}

int _tmain(int argc, _TCHAR* argv[])
{
    AvxBlendFloat();
    AvxBlendByte();
    return 0;
}
