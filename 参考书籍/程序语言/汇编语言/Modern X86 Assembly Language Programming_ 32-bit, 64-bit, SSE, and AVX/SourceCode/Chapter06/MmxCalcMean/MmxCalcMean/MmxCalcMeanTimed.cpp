#include "stdafx.h"
#include "MmxCalcMean.h"
#include "ThreadTimer.h"
#include <stdlib.h>

void MmxCalcMeanTimed()
{
    // Force current thread to execute on a single processor
    ThreadTimer::SetThreadAffinityMask();

    const int n = NUM_ELEMENTS;
    UINT8* x = new UINT8[n];

    srand(SRAND_SEED);
    for (int i = 0; i < n; i++)
        x[i] = rand() % 256;

    const int num_it = 100;
    const int num_alg = 2;
    const double et_scale = 1.0e6;
    double et[num_it][num_alg];
    Uint32 sum1, sum2;
    double mean1, mean2;
    ThreadTimer tt;

    for (int i = 0; i < num_it; i++)
    {
        tt.Start();
        MmxCalcMeanCpp(x, n, &sum1, &mean1);
        tt.Stop();
        et[i][0] = tt.GetElapsedTime() * et_scale;
    }

    for (int i = 0; i < num_it; i++)
    {
        tt.Start();
        MmxCalcMean_(x, n, &sum2, &mean2);
        tt.Stop();
        et[i][1] = tt.GetElapsedTime() * et_scale;
    }

    const char* fn = "__MmxCalcMeanTimed.csv";
    ThreadTimer::SaveElapsedTimeMatrix(fn, (double*)et, num_it, num_alg);

    printf("\nResults for MmxCalcMeanTimed()\n");
    printf("sum1: %u  mean1: %12.6lf\n", sum1, mean1);
    printf("sum2: %u  mean2: %12.6lf\n", sum2, mean2);
    printf("\nBenchmark times saved to file %s\n", fn);
    delete[] x;
}