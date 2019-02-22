#include "stdafx.h"
#include "MiscDefs.h"

extern "C" void AvxGprCountZeroBits_(Uint32 x, Uint32* lzcnt, Uint32* tzcnt);
extern "C" Uint32 AvxGprBextr_(Uint32 x, Uint8 start, Uint8 length);
extern "C" Uint32 AvxGprAndNot_(Uint32 x, Uint32 y);

void AvxGprCountZeroBits(void)
{
    const int n = 5;
    Uint32 x[n] = { 0x001000008, 0x00008000, 0x8000000, 0x00000001, 0 };

    printf("\nResults for AvxGprCountZeroBits()\n");
    for (int i = 0; i < n; i++)
    {
        Uint32 lzcnt, tzcnt;

        AvxGprCountZeroBits_(x[i], &lzcnt, &tzcnt);
        printf("x: 0x%08X  ", x[i]);
        printf("lzcnt: %2u  ", lzcnt);
        printf("tzcnt: %2u\n", tzcnt);
    }
}

void AvxGprExtractBitField(void)
{
    const int n = 3;
    Uint32 x[n] = { 0x12345678, 0x80808080, 0xfedcba98 };
    Uint8 start[n] = { 4, 7, 24 };
    Uint8 len[n] = { 16, 9, 8 };

    printf("\nResults for AvxGprExtractBitField()\n");
    for (int i = 0; i < n; i++)
    {
        Uint32 bextr = AvxGprBextr_(x[i], start[i], len[i]);

        printf("x: 0x%08X  ", x[i]);
        printf("start: %2u  ", start[i]);
        printf("len:  %2u  ", len[i]);
        printf("bextr: 0x%08X\n", bextr);
    }
}

void AvxGprAndNot(void)
{
    const int n = 3;
    Uint32 x[n] = { 0xf000000f, 0xff00ff00, 0xaaaaaaaa };
    Uint32 y[n] = { 0x12345678, 0x12345678, 0xffaa5500 };

    printf("\nResults for AvxGprAndNot()\n");
    for (int i = 0; i < n; i++)
    {
        Uint32 andn = AvxGprAndNot_(x[i], y[i]);
        printf("x: 0x%08X  y: 0x%08X  z: 0x%08X\n", x[i], y[i], andn);
    }
}

int _tmain(int argc, _TCHAR* argv[])
{
    AvxGprCountZeroBits();
    AvxGprExtractBitField();
    AvxGprAndNot();
    return 0;
}
