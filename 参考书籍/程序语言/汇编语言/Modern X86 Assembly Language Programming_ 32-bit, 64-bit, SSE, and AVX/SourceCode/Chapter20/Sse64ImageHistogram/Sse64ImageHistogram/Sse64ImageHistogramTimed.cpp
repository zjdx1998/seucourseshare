#include "stdafx.h"
#include "Sse64ImageHistogram.h"
#include "ThreadTimer.h"
#include <malloc.h>

void Sse64ImageHistogramTimed(void)
{
    ThreadTimer::SetThreadAffinityMask();

    ImageBuffer ib(L"..\\..\\..\\DataFiles\\TestImage1.bmp");
    Uint32 num_pixels = ib.GetNumPixels();
    Uint8* pixel_buffer = (Uint8*)ib.GetPixelBuffer();

    const int num_it = 100;
    const int num_alg = 2;
    const double et_scale = 1.0e6;
    double et[num_it][num_alg];
    Uint32* histograms[num_alg];
    ThreadTimer tt;

    for (int i = 0; i < num_alg; i++)
        histograms[i] = (Uint32*)_aligned_malloc(256 * sizeof(Uint32), 16);

    for (int i = 0; i < num_it; i++)
    {
        tt.Start();
        Sse64ImageHistogramCpp(histograms[0], pixel_buffer, num_pixels);
        tt.Stop();
        et[i][0] = tt.GetElapsedTime() * et_scale;
    }

    for (int i = 0; i < num_it; i++)
    {
        tt.Start();
        Sse64ImageHistogram_(histograms[1], pixel_buffer, num_pixels);
        tt.Stop();
        et[i][1] = tt.GetElapsedTime() * et_scale;
    }

    const char* fn = "__Sse64ImageHistogramTimed.csv";
    ThreadTimer::SaveElapsedTimeMatrix(fn, (double*)et, num_it, num_alg);

    printf("\nBenchmark times saved to file %s\n", fn);

    for (int i = 0; i < num_alg; i++)
        _aligned_free(histograms[i]);
}
