#include "stdafx.h"
#include "MmxCalcMinMax.h"
#include <stdlib.h>

extern "C" int NMIN = 16;           // Minimum number of array elements

bool MmxCalcMinMaxCpp(Uint8* x, int n, Uint8* x_min, Uint8* x_max)
{
    if ((n < NMIN) || ((n & 0x0f) != 0))
        return false;

    Uint8 x_min_temp = 0xff;
    Uint8 x_max_temp = 0;

    for (int i = 0; i < n; i++)
    {
        Uint8 val = *x++;

        if (val < x_min_temp)
            x_min_temp = val;
        else if (val > x_max_temp)
            x_max_temp = val;
    }

    *x_min = x_min_temp;
    *x_max = x_max_temp;
    return true;
}

void MmxCalcMinMax()
{
    const int n = NUM_ELEMENTS;
    Uint8* x = new Uint8[n];

    // Initialize test array with known min and max values
    srand(SRAND_SEED);
    for (int i = 0; i < n; i++)
        x[i] = (Uint8)((rand() % 240) + 10);

    x[n / 4] = 4;
    x[n / 2] = 252;

    bool rc1, rc2;
    Uint8 x_min1 = 0, x_max1 = 0;
    Uint8 x_min2 = 0, x_max2 = 0;

    rc1 = MmxCalcMinMaxCpp(x, n, &x_min1, &x_max1);
    rc2 = MmxCalcMinMax_(x, n, &x_min2, &x_max2);

    printf("\nResults for MmxCalcMinMax()\n");
    printf("rc1: %d  x_min1: %3u  x_max1: %3u\n", rc1, x_min1, x_max1);
    printf("rc2: %d  x_min2: %3u  x_max2: %3u\n", rc2, x_min2, x_max2);
    delete[] x;
}

int _tmain(int argc, _TCHAR* argv[])
{
    MmxCalcMinMax();
    MmxCalcMinMaxTimed();
    return 0;
}
