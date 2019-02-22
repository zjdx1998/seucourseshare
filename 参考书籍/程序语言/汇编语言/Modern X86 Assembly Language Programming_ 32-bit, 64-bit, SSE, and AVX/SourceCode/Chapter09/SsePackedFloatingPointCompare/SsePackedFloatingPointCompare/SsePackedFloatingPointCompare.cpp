#include "stdafx.h"
#include "XmmVal.h"
#include <limits>
using namespace std;

extern "C" void SsePfpCompareFloat_(const XmmVal* a, const XmmVal* b, XmmVal c[8]);

const char* CmpStr[8] =
{
    "EQ", "LT", "LE", "UNORDERED", "NE", "NLT", "NLE", "ORDERED"
};

void SsePfpCompareFloat(void)
{
    __declspec(align(16)) XmmVal a;
    __declspec(align(16)) XmmVal b;
    __declspec(align(16)) XmmVal c[8];
    char buff[256];

    a.r32[0] = 2.0;         b.r32[0] = 1.0;
    a.r32[1] = 7.0;         b.r32[1] = 12.0;
    a.r32[2] = -6.0;        b.r32[2] = -6.0;
    a.r32[3] = 3.0;         b.r32[3] = 8.0;

    for (int i = 0; i < 2; i++)
    {
        if (i == 1)
            a.r32[0] = numeric_limits<float>::quiet_NaN();

        SsePfpCompareFloat_(&a, &b, c);

        printf("\nResults for SsePfpCompareFloat_ (Iteration %d)\n", i);
        printf("a: %s\n", a.ToString_r32(buff, sizeof(buff)));
        printf("b: %s\n", b.ToString_r32(buff, sizeof(buff)));
        printf("\n");

        for (int j = 0; j < 8; j++)
        {
            char* s =  c[j].ToString_x32(buff, sizeof(buff));
            printf("%10s: %s\n", CmpStr[j], s);
        }
    }
}

int _tmain(int argc, _TCHAR* argv[])
{
    SsePfpCompareFloat();
    return 0;
}
