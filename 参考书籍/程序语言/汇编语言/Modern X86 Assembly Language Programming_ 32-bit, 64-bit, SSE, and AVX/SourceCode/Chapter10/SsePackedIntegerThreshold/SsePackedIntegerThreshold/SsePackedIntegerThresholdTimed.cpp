#include "stdafx.h"
#include "SsePackedIntegerThreshold.h"
#include "ThreadTimer.h"

void SsePiThresholdTimed(void)
{
    ThreadTimer::SetThreadAffinityMask();

    wchar_t* fn_src = L"..\\..\\..\\DataFiles\\TestImage2.bmp";
    ImageBuffer* im_src = new ImageBuffer(fn_src);
    ImageBuffer* im_mask0 = new ImageBuffer(*im_src);
    ImageBuffer* im_mask1 = new ImageBuffer(*im_src);
    ITD itd0, itd1;

    itd0.PbSrc = (Uint8*)im_src->GetPixelBuffer();
    itd0.PbMask = (Uint8*)im_mask0->GetPixelBuffer();
    itd0.NumPixels = im_src->GetNumPixels();
    itd0.Threshold = TEST_THRESHOLD;

    itd1.PbSrc = (Uint8*)im_src->GetPixelBuffer();
    itd1.PbMask = (Uint8*)im_mask1->GetPixelBuffer();
    itd1.NumPixels = im_src->GetNumPixels();
    itd1.Threshold = TEST_THRESHOLD;

    const int num_it = 100;
    const int num_alg = 2;
    const double et_scale = 1.0e6;
    double et[num_it][num_alg];
    ThreadTimer tt;

    for (int i = 0; i < num_it; i++)
    {
        tt.Start();
        SsePiThresholdCpp(&itd0);
        SsePiCalcMeanCpp(&itd0);
        tt.Stop();
        et[i][0] = tt.GetElapsedTime() * et_scale;
    }

    for (int i = 0; i < num_it; i++)
    {
        tt.Start();
        SsePiThreshold_(&itd1);
        SsePiCalcMean_(&itd1);
        tt.Stop();
        et[i][1] = tt.GetElapsedTime() * et_scale;
    }

    const char* fn = "__SsePackedImageThresholdTimed.csv";
    ThreadTimer::SaveElapsedTimeMatrix(fn, (double*)et, num_it, num_alg);

    printf("\nBenchmark times saved to file %s\n", fn);

    delete im_src;
    delete im_mask0;
    delete im_mask1;
}
