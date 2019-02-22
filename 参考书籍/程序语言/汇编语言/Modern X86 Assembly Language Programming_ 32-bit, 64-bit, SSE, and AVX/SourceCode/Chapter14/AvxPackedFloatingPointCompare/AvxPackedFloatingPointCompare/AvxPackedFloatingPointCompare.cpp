#include "stdafx.h"
#include "YmmVal.h"
#include <limits>
using namespace std;

extern "C" void AvxPfpCompare_(const YmmVal* a, const YmmVal* b, YmmVal c[8]);

int _tmain(int argc, _TCHAR* argv[])
{
    char buff[256];
    __declspec(align(32)) YmmVal a;
    __declspec(align(32)) YmmVal b;
    __declspec(align(32)) YmmVal c[8];

    const char* instr_names[8] =
    {
        "vcmpeqpd", "vcmpneqpd", "vcmpltpd", "vcmplepd",
        "vcmpgtpd", "vcmpgepd", "vcmpordpd", "vcmpunordpd"
    };

    a.r64[0] = 42.125;
    a.r64[1] = -36.875;
    a.r64[2] = 22.95;
    a.r64[3] = 3.75;

    b.r64[0] = -0.0625;
    b.r64[1] = -67.375;
    b.r64[2] = 22.95;
    b.r64[3] = numeric_limits<double>::quiet_NaN();

    AvxPfpCompare_(&a, &b, c);

    printf("Results for AvxPackedFloatingPointCompare\n");
    printf("a: %s\n", a.ToString_r64(buff, sizeof(buff), false));
    printf("a: %s\n", a.ToString_r64(buff, sizeof(buff), true));
    printf("\n");
    printf("b: %s\n", b.ToString_r64(buff, sizeof(buff), false));
    printf("b: %s\n", b.ToString_r64(buff, sizeof(buff), true));

    for (int i = 0; i < 8; i++)
    {
        printf("\n%s results\n", instr_names[i]);
        printf("  %s\n", c[i].ToString_x64(buff, sizeof(buff), false));
        printf("  %s\n", c[i].ToString_x64(buff, sizeof(buff), true));
    }

    return 0;
}
