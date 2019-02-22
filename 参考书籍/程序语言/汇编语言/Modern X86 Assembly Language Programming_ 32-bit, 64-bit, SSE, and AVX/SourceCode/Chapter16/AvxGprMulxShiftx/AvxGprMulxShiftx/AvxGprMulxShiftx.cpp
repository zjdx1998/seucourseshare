#include "stdafx.h"
#include "MiscDefs.h"

extern "C" Uint64 AvxGprMulx_(Uint32 a, Uint32 b, Uint8 flags[2]);
extern "C" void AvxGprShiftx_(Int32 x, Uint32 count, Int32 results[3]);

void AvxGprMulx(void)
{
    const int n = 3;
    Uint32 a[n] = {64, 3200, 100000000};
    Uint32 b[n] = {1001, 12, 250000000};

    printf("Results for AvxGprMulx()\n");
    for (int i = 0; i < n; i++)
    {
        Uint8 flags[2];
        Uint64 c = AvxGprMulx_(a[i], b[i], flags);

        printf("Test case %d\n", i);
        printf("  a: %u  b: %u  c: %llu\n", a[i], b[i], c);
        printf("  status flags before mulx: 0x%02X\n", flags[0]);
        printf("  status flags after mulx:  0x%02X\n", flags[1]);
    }
}

void AvxGprShiftx(void)
{
    const int n = 4;
    Int32 x[n] = { 0x00000008, 0x80000080, 0x00000040, 0xfffffc10 };
    Uint32 count[n] = { 2, 5, 3, 4 };

    printf("\nResults for AvxGprShiftx()\n");
    for (int i = 0; i < n; i++)
    {
        Int32 results[3];

        AvxGprShiftx_(x[i], count[i], results);
        printf("Test case %d\n", i);
        printf("  x:    0x%08X (%11d) count: %u\n", x[i], x[i], count[i]);
        printf("  sarx: 0x%08X (%11d)\n", results[0], results[0]);
        printf("  shlx: 0x%08X (%11d)\n", results[1], results[1]);
        printf("  shrx: 0x%08X (%11d)\n", results[2], results[2]);
    }
}

int _tmain(int argc, _TCHAR* argv[])
{
    AvxGprMulx();
    AvxGprShiftx();
    return 0;
}
