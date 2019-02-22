#include "stdafx.h"
#include "AvxFma.h"
#include "ThreadTimer.h"
#include <malloc.h>
#include <stdlib.h>

void AvxFmaTimed(void)
{
    ThreadTimer::SetThreadAffinityMask();

    const int num_it = 100;
    const int num_alg = 4;
    const double et_scale = 1.0e6;
    double et[num_it][num_alg];
    ThreadTimer tt;
    const Uint32 n = 50000;
    float* x = (float*)_aligned_malloc(n * sizeof(float), 32);
    float* y = (float*)_aligned_malloc(n * sizeof(float), 32);
    const float sm5_mask[] = { 0.0625f, 0.25f, 0.375f, 0.25f, 0.0625f };

    if (!AvxFmaInitX(x, n))
    {
        printf("Data initialization failed\n");
        return;
    }

    for (int i = 0; i < num_it; i++)
    {
        tt.Start();
        AvxFmaSmooth5Cpp(x, y, n, sm5_mask);
        tt.Stop();
        et[i][0] = tt.GetElapsedTime() * et_scale;

        tt.Start();
        AvxFmaSmooth5a_(x, y, n, sm5_mask);
        tt.Stop();
        et[i][1] = tt.GetElapsedTime() * et_scale;

        tt.Start();
        AvxFmaSmooth5b_(x, y, n, sm5_mask);
        tt.Stop();
        et[i][2] = tt.GetElapsedTime() * et_scale;

        tt.Start();
        AvxFmaSmooth5c_(x, y, n, sm5_mask);
        tt.Stop();
        et[i][3] = tt.GetElapsedTime() * et_scale;
    }

    const char* fn = "__AvxFmaTimed.csv";
    ThreadTimer::SaveElapsedTimeMatrix(fn, (double*)et, num_it, num_alg);

    printf("\nBenchmark times saved to file %s\n", fn);

    _aligned_free(x);
    _aligned_free(y);
}
