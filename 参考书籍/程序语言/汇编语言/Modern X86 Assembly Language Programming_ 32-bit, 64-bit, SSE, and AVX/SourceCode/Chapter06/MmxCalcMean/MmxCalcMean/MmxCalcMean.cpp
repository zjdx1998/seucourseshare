#include "stdafx.h"
#include "MmxCalcMean.h"
#include <stdlib.h>

extern "C" int NMIN = 16;               // Minimum number of elements
extern "C" int NMAX = 16777216;         // Maximum number of elements

bool MmxCalcMeanCpp(const Uint8* x, int n, Uint32* sum_x, double* mean_x)
{
    if ((n < NMIN) || (n > NMAX) || ((n & 0x0f) != 0))
        return false;

    Uint32 sum_x_temp = 0;
    for (int i = 0; i < n; i++)
        sum_x_temp += x[i];

    *sum_x = sum_x_temp;
    *mean_x = (double)sum_x_temp / n;
    return true;
}

void MmxCalcMean()
{
    const int n = NUM_ELEMENTS;
    Uint8* x = new Uint8[n];

    srand(SRAND_SEED);
    for (int i = 0; i < n; i++)
        x[i] = rand() % 256;

    bool rc1, rc2;
    Uint32 sum_x1 = 0, sum_x2 = 0;
    double mean_x1 = 0, mean_x2 = 0;

    rc1 = MmxCalcMeanCpp(x, n, &sum_x1, &mean_x1);
    rc2 = MmxCalcMean_(x, n, &sum_x2, &mean_x2);

    printf("\nResults for MmxCalcMean()\n");
    printf("rc1: %d sum_x1: %u mean_x1: %12.6lf\n", rc1, sum_x1, mean_x1);
    printf("rc2: %d sum_x2: %u mean_x2: %12.6lf\n", rc2, sum_x2, mean_x2);
    delete[] x;
}

int _tmain(int argc, _TCHAR* argv[])
{
    MmxCalcMean();
    MmxCalcMeanTimed();
    return 0;
}
