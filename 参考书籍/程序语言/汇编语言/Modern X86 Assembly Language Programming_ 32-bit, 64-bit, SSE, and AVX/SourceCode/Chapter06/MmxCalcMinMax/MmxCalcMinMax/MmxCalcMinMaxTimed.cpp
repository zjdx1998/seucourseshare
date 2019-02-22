#include "stdafx.h"
#include "MmxCalcMinMax.h"
#include "ThreadTimer.h"
#include <stdlib.h>

void MmxCalcMinMaxTimed(void)
{
    // Force current thread to execute on a single processor
    ThreadTimer::SetThreadAffinityMask();

    const int n = NUM_ELEMENTS;
    Uint8* x = new Uint8[n];

    // Initialize test array with known min and max values
    srand(SRAND_SEED);
    for (int i = 0; i < n; i++)
        x[i] = (Uint8)((rand() % 240) + 10);

    x[n / 4] = 4;
    x[n / 2] = 252;

    const int num_it = 100;
    const int num_alg = 2;
    const double et_scale = 1.0e6;
    double et[num_it][num_alg];
    Uint8 x_min1 = 0, x_max1 = 0;
    Uint8 x_min2 = 0, x_max2 = 0;
    ThreadTimer tt;

    for (int i = 0; i < num_it; i++)
    {
        tt.Start();
        MmxCalcMinMaxCpp(x, n, &x_min1, &x_max1);
        tt.Stop();
        et[i][0] = tt.GetElapsedTime() * et_scale;
    }

    for (int i = 0; i < num_it; i++)
    {
        tt.Start();
        MmxCalcMinMax_(x, n, &x_min2, &x_max2);
        tt.Stop();
        et[i][1] = tt.GetElapsedTime() * et_scale;
    }

    const char* fn = "__MmxCalcMinMaxTimed.csv";
    ThreadTimer::SaveElapsedTimeMatrix(fn, (double*)et, num_it, num_alg);

    printf("\nResults for MmxCalcMinMaxTimed()\n");
    printf("x_min1: %3u  x_max1: %3d\n", x_min1, x_max1);
    printf("x_min2: %3u  x_max2: %3d\n", x_min2, x_max2);
    printf("\nBenchmark times saved to file %s\n", fn);
    delete[] x;
}
