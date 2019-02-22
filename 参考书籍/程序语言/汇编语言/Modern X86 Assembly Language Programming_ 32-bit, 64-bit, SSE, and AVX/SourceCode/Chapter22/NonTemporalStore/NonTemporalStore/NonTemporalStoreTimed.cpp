#include "stdafx.h"
#include "NonTemporalStore.h"
#include "ThreadTimer.h"
#include <malloc.h>
#include <stdlib.h>

void NonTemporalStoreTimed(void)
{
    ThreadTimer::SetThreadAffinityMask();

    const int num_it = 500;
    const int num_alg = 3;
    const double et_scale = 1.0e6;
    double et[num_it][num_alg];
    ThreadTimer tt;

    const int n = 1000000;
    const int align = 16;
    float* a = (float*)_aligned_malloc(n * sizeof(float), align);
    float* b = (float*)_aligned_malloc(n * sizeof(float), align);
    float* c1 = (float*)_aligned_malloc(n * sizeof(float), align);
    float* c2a = (float*)_aligned_malloc(n * sizeof(float), align);
    float* c2b = (float*)_aligned_malloc(n * sizeof(float), align);

    srand(67);
    for (int i = 0; i < n; i++)
    {
        a[i] = (float)(rand() % 100);
        b[i] = (float)(rand() % 100);
    }

    for (int i = 0; i < num_it; i++)
    {
        tt.Start();
        CalcResultCpp(c1, a, b, n);
        tt.Stop();
        et[i][0] = tt.GetElapsedTime() * et_scale;
    }

    for (int i = 0; i < num_it; i++)
    {
        tt.Start();
        CalcResultA_(c2a, a, b, n);
        tt.Stop();
        et[i][1] = tt.GetElapsedTime() * et_scale;
    }

    for (int i = 0; i < num_it; i++)
    {
        tt.Start();
        CalcResultB_(c2b, a, b, n);
        tt.Stop();
        et[i][2] = tt.GetElapsedTime() * et_scale;
    }

#ifdef _WIN64
    const char* fn = "__NonTemporalStore64.csv";
#else
    const char* fn = "__NonTemporalStore32.csv";
#endif

    if (!CompareResults(c1, c2a, c2b, n, false))
        printf("NonTemporalStoreTimed() - array compare FAILED\n");

    ThreadTimer::SaveElapsedTimeMatrix(fn, (double*)et, num_it, num_alg);
    printf("\nBenchmark times saved to file %s\n", fn);

    _aligned_free(a);
    _aligned_free(b);
    _aligned_free(c1);
    _aligned_free(c2a);
    _aligned_free(c2b);
}
