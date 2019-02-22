#include "stdafx.h"
#include "AvxPackedIntegerPixelClip.h"
#include <malloc.h>
#include <memory.h>
#include <stdlib.h>

bool AvxPiPixelClipCpp(PcData* pc_data)
{
    Uint32 num_pixels = pc_data->NumPixels;
    Uint8* src = pc_data->Src;
    Uint8* des = pc_data->Des;

    if ((num_pixels < 32) || ((num_pixels & 0x1f) != 0))
        return false;

    if (((uintptr_t)src & 0x1f) != 0)
        return false;
    if (((uintptr_t)des & 0x1f) != 0)
        return false;

    Uint8 thresh_lo = pc_data->ThreshLo;
    Uint8 thresh_hi = pc_data->ThreshHi;
    Uint32 num_clipped_pixels = 0;

    for (Uint32 i = 0; i < num_pixels; i++)
    {
        Uint8 pixel = src[i];

        if (pixel < thresh_lo)
        {
            des[i] = thresh_lo;
            num_clipped_pixels++;
        }
        else if (pixel > thresh_hi)
        {
            des[i] = thresh_hi;
            num_clipped_pixels++;
        }
        else
            des[i] = src[i];
    }

    pc_data->NumClippedPixels = num_clipped_pixels;
    return true;
}

void AvxPackedIntegerPixelClip(void)
{
    const Uint8 thresh_lo = 10;
    const Uint8 thresh_hi = 245;
    const Uint32 num_pixels = 4 * 1024 * 1024;
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

    AvxPiPixelClipCpp(&pc_data1);
    AvxPiPixelClip_(&pc_data2);

    printf("Results for AvxPackedIntegerPixelClip\n");

    if (pc_data1.NumClippedPixels != pc_data2.NumClippedPixels)
        printf("  NumClippedPixels compare error!\n");

    printf("  NumClippedPixels1: %u\n", pc_data1.NumClippedPixels);
    printf("  NumClippedPixels2: %u\n", pc_data2.NumClippedPixels);

    if (memcmp(des1, des2, num_pixels) == 0)
        printf("  Destination buffer memory compare passed\n");
    else
        printf("  Destination buffer memory compare failed!\n");

    _aligned_free(src);
    _aligned_free(des1);
    _aligned_free(des2);
}

int _tmain(int argc, _TCHAR* argv[])
{
    AvxPackedIntegerPixelClip();
    AvxPackedIntegerPixelClipTimed();
    return 0;
}
