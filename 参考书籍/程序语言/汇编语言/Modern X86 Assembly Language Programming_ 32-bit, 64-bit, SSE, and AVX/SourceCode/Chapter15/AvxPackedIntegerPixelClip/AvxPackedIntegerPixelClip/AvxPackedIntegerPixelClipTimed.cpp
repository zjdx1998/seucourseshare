#include "stdafx.h"
#include "AvxPackedIntegerPixelClip.h"
#include "ThreadTimer.h"
#include <malloc.h>
#include <stdlib.h>

void AvxPackedIntegerPixelClipTimed(void)
{
    ThreadTimer::SetThreadAffinityMask();

    const int num_it = 100;
    const int num_alg = 2;
    const double et_scale = 1.0e6;
    double et[num_it][num_alg];
    ThreadTimer tt;

    const Uint8 thresh_lo = 10;
    const Uint8 thresh_hi = 245;
    const Uint32 num_pixels = 8 * 1024 * 1024;
    Uint8* src = (Uint8*)_aligned_malloc(num_pixels, 32);
    Uint8* des1 = (Uint8*)_aligned_malloc(num_pixels, 32);
    Uint8* des2 = (Uint8*)_aligned_malloc(num_pixels, 32);

    srand(157);
    for (int i = 0; i < num_pixels; i++)
        src[i] = (Uint8)(rand() % 256);

    PcData pc_data1;
    PcData pc_data2;

    pc_data1.Src = pc_data2.Src = src;
    pc_data1.Des = des1;
    pc_data2.Des = des2;
    pc_data1.NumPixels = pc_data2.NumPixels = num_pixels;
    pc_data1.ThreshLo = pc_data2.ThreshLo = thresh_lo;
    pc_data1.ThreshHi = pc_data2.ThreshHi = thresh_hi;

    for (int i = 0; i < num_it; i++)
    {
        tt.Start();
        AvxPiPixelClipCpp(&pc_data1);
        tt.Stop();
        et[i][0] = tt.GetElapsedTime() * et_scale;
    }

    for (int i = 0; i < num_it; i++)
    {
        tt.Start();
        AvxPiPixelClip_(&pc_data2);
        tt.Stop();
        et[i][1] = tt.GetElapsedTime() * et_scale;
    }

    const char* fn = "__AvxPackedIntegerPixelClipTimed.csv";
    ThreadTimer::SaveElapsedTimeMatrix(fn, (double*)et, num_it, num_alg);

    printf("\nBenchmark times saved to file %s\n", fn);

    _aligned_free(src);
    _aligned_free(des1);
    _aligned_free(des2);
}
