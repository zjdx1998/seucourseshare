#include "stdafx.h"
#include "XmmVal.h"
#define _USE_MATH_DEFINES
#include <math.h>

// The order of values in the following enum must match the table
// that's defined in SsePackedFloatingPointConversions_.asm.
enum CvtOp : unsigned int
{
    Cvtdq2ps,           // Packed signed doubleword to SPFP
    Cvtdq2pd,           // Packed signed doubleword to DPFP
    Cvtps2dq,           // Packed SPFP to signed doubleword
    Cvtpd2dq,           // Packed DPFP to signed doubleword
    Cvtps2pd,           // Packed SPFP to DPFP
    Cvtpd2ps            // Packed DPFP to SPFP
};

extern "C" void SsePfpConvert_(const XmmVal* a, XmmVal* b, CvtOp cvt_op);

void SsePfpConversions32(void)
{
    _declspec(align(16)) XmmVal a;
    _declspec(align(16)) XmmVal b;
    char buff[256];

    a.i32[0] = 10;
    a.i32[1] = -500;
    a.i32[2] = 600;
    a.i32[3] = -1024;
    SsePfpConvert_(&a, &b, CvtOp::Cvtdq2ps);
    printf("\nResults for CvtOp::Cvtdq2ps\n");
    printf("  a: %s\n", a.ToString_i32(buff, sizeof(buff)));
    printf("  b: %s\n", b.ToString_r32(buff, sizeof(buff)));

    a.r32[0] = 1.0f / 3.0f;
    a.r32[1] = 2.0f / 3.0f;
    a.r32[2] = -a.r32[0] * 2.0f;
    a.r32[3] = -a.r32[1] * 2.0f;
    SsePfpConvert_(&a, &b, CvtOp::Cvtps2dq);
    printf("\nResults for CvtOp::Cvtps2dq\n");
    printf("  a: %s\n", a.ToString_r32(buff, sizeof(buff)));
    printf("  b: %s\n", b.ToString_i32(buff, sizeof(buff)));

    // cvtps2pd converts the two low-order SPFP values of 'a'
    a.r32[0] = 1.0f / 7.0f;
    a.r32[1] = 2.0f / 9.0f;
    a.r32[2] = 0;
    a.r32[3] = 0;
    SsePfpConvert_(&a, &b, CvtOp::Cvtps2pd);
    printf("\nResults for CvtOp::Cvtps2pd\n");
    printf("  a: %s\n", a.ToString_r32(buff, sizeof(buff)));
    printf("  b: %s\n", b.ToString_r64(buff, sizeof(buff)));
}

void SsePfpConversions64(void)
{
    _declspec(align(16)) XmmVal a;
    _declspec(align(16)) XmmVal b;
    char buff[256];

    // cvtdq2pd converts the two low-order doubleword integers of 'a'
    a.i32[0] = 10;
    a.i32[1] = -20;
    a.i32[2] = 0;
    a.i32[3] = 0;
    SsePfpConvert_(&a, &b, CvtOp::Cvtdq2pd);
    printf("\nResults for CvtOp::Cvtdq2pd\n");
    printf("  a: %s\n", a.ToString_i32(buff, sizeof(buff)));
    printf("  b: %s\n", b.ToString_r64(buff, sizeof(buff)));

    // cvtpd2dq sets the two high-order doublewords of 'b' to zero
    a.r64[0] = M_PI;
    a.r64[1] = M_E;
    SsePfpConvert_(&a, &b, CvtOp::Cvtpd2dq);
    printf("\nResults for CvtOp::Cvtpd2dq\n");
    printf("  a: %s\n", a.ToString_r64(buff, sizeof(buff)));
    printf("  b: %s\n", b.ToString_i32(buff, sizeof(buff)));

    // cvtpd2ps sets the two high-order SPFP values of 'b' to zero
    a.r64[0] = M_SQRT2;
    a.r64[1] = M_SQRT1_2;
    SsePfpConvert_(&a, &b, CvtOp::Cvtpd2ps);
    printf("\nResults for CvtOp::Cvtpd2ps\n");
    printf("  a: %s\n", a.ToString_r64(buff, sizeof(buff)));
    printf("  b: %s\n", b.ToString_r32(buff, sizeof(buff)));
}

int _tmain(int argc, _TCHAR* argv[])
{
    SsePfpConversions32();
    SsePfpConversions64();
    return 0;
}
