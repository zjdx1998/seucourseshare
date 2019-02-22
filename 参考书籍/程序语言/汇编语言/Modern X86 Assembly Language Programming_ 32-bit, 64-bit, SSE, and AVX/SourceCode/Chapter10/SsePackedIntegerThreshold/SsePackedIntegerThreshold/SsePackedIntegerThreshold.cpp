#include "stdafx.h"
#include "SsePackedIntegerThreshold.h"
#include <stddef.h>

extern "C" Uint32 NUM_PIXELS_MAX = 16777216;

bool SsePiThresholdCpp(ITD* itd)
{
    Uint8* pb_src = itd->PbSrc;
    Uint8* pb_mask = itd->PbMask;
    Uint8 threshold = itd->Threshold;
    Uint32 num_pixels = itd->NumPixels;

    // Make sure num_pixels is valid
    if ((num_pixels == 0) || (num_pixels > NUM_PIXELS_MAX))
        return false;
    if ((num_pixels & 0x1f) != 0)
        return false;

    // Make sure image buffers are properly aligned
    if (((uintptr_t)pb_src & 0xf) != 0)
        return false;
    if (((uintptr_t)pb_mask & 0xf) != 0)
        return false;

    // Threshold the image
    for (Uint32 i = 0; i < num_pixels; i++)
        *pb_mask++ = (*pb_src++ > threshold) ? 0xff : 0x00;

    return true;
}

bool SsePiCalcMeanCpp(ITD* itd)
{
    Uint8* pb_src = itd->PbSrc;
    Uint8* pb_mask = itd->PbMask;
    Uint32 num_pixels = itd->NumPixels;

    // Make sure num_pixels is valid
    if ((num_pixels == 0) || (num_pixels > NUM_PIXELS_MAX))
        return false;
    if ((num_pixels & 0x1f) != 0)
        return false;

    // Make sure image buffers are properly aligned
    if (((uintptr_t)pb_src & 0xf) != 0)
        return false;
    if (((uintptr_t)pb_mask & 0xf) != 0)
        return false;

    // Calculate mean of masked pixels
    Uint32 sum_masked_pixels = 0;
    Uint32 num_masked_pixels = 0;

    for (Uint32 i = 0; i < num_pixels; i++)
    {
        Uint8 mask_val = *pb_mask++;
        num_masked_pixels += mask_val & 1;
        sum_masked_pixels += (*pb_src++ & mask_val);
    }

    itd->NumMaskedPixels = num_masked_pixels;
    itd->SumMaskedPixels = sum_masked_pixels;
    
    if (num_masked_pixels > 0)
        itd->MeanMaskedPixels = (double)sum_masked_pixels / num_masked_pixels;
    else
        itd->MeanMaskedPixels = -1.0;

    return true;
}

void SsePiThreshold()
{
    wchar_t* fn_src = L"..\\..\\..\\DataFiles\\TestImage2.bmp";
    wchar_t* fn_mask1 = L"__TestImage2_Mask1.bmp";
    wchar_t* fn_mask2 = L"__TestImage2_Mask2.bmp";
    ImageBuffer* im_src = new ImageBuffer(fn_src);
    ImageBuffer* im_mask1 = new ImageBuffer(*im_src, false);
    ImageBuffer* im_mask2 = new ImageBuffer(*im_src, false);
    ITD itd1, itd2;

    itd1.PbSrc = (Uint8*)im_src->GetPixelBuffer();
    itd1.PbMask = (Uint8*)im_mask1->GetPixelBuffer();
    itd1.NumPixels = im_src->GetNumPixels();
    itd1.Threshold = TEST_THRESHOLD;

    itd2.PbSrc = (Uint8*)im_src->GetPixelBuffer();
    itd2.PbMask = (Uint8*)im_mask2->GetPixelBuffer();
    itd2.NumPixels = im_src->GetNumPixels();
    itd2.Threshold = TEST_THRESHOLD;

    bool rc1 = SsePiThresholdCpp(&itd1);
    bool rc2 = SsePiThreshold_(&itd2);

    if (!rc1 || !rc2)
    {
        printf("Bad Threshold return code: rc1=%d, rc2=%d\n", rc1, rc2);
        return;
    }

    im_mask1->SaveToBitmapFile(fn_mask1);
    im_mask2->SaveToBitmapFile(fn_mask2);

    // Calculate mean of masked pixels
    rc1 = SsePiCalcMeanCpp(&itd1);
    rc2 = SsePiCalcMean_(&itd2);

    if (!rc1 || !rc2)
    {
        printf("Bad CalcMean return code: rc1=%d, rc2=%d\n", rc1, rc2);
        return;
    }

    printf("Results for SsePackedIntegerThreshold\n\n");
    printf("                           C++       X86-SSE\n");
    printf("--------------------------------------------\n");
    printf("SumPixelsMasked:  ");
    printf("%12u  %12u\n", itd1.SumMaskedPixels, itd2.SumMaskedPixels);
    printf("NumPixelsMasked:  ");
    printf("%12u  %12u\n", itd1.NumMaskedPixels, itd2.NumMaskedPixels);
    printf("MeanPixelsMasked: ");
    printf("%12.6lf  %12.6lf\n", itd1.MeanMaskedPixels, itd2.MeanMaskedPixels);

    delete im_src;
    delete im_mask1;
    delete im_mask2;
}

int _tmain(int argc, _TCHAR* argv[])
{
    try
    {
        SsePiThreshold();
        SsePiThresholdTimed();
    }

    catch (...)
    {
        printf("Unexpected exception has occurred!\n");
        printf("File: %s (_tmain)\n", __FILE__);
    }
    return 0;
}
