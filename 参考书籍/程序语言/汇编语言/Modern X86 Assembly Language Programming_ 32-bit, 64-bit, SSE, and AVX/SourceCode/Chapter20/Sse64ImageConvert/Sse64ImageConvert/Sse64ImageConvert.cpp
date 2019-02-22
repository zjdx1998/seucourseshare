#include "stdafx.h"
#include "MiscDefs.h"
#include <malloc.h>
#include <stdlib.h>

extern "C" Uint32 NUM_PIXELS_MAX = 16777216;
extern "C" bool ImageUint8ToFloat_(float* des, const Uint8* src, Uint32 num_pixels);
extern "C" bool ImageFloatToUint8_(Uint8* des, const float* src, Uint32 num_pixels);

bool ImageUnit8ToFloatCpp(float* des, const Uint8* src, Uint32 num_pixels)
{
    // Make sure num_pixels is valid
    if ((num_pixels == 0) || (num_pixels > NUM_PIXELS_MAX))
        return false;
    if (num_pixels % 32 != 0)
        return false;

    // Make sure src and des are aligned to a 16-byte boundary
    if (((uintptr_t)src & 0xf) != 0)
        return false;
    if (((uintptr_t)des & 0xf) != 0)
        return false;

    // Convert the image
    for (Uint32 i = 0; i < num_pixels; i++)
        des[i] = src[i] / 255.0f;

    return true;
}

bool ImageFloatToUint8Cpp(Uint8* des, const float* src, Uint32 num_pixels)
{
    // Make sure num_pixels is valid
    if ((num_pixels == 0) || (num_pixels > NUM_PIXELS_MAX))
        return false;
    if (num_pixels % 32 != 0)
        return false;

    // Make sure src and des are aligned to a 16-byte boundary
    if (((uintptr_t)src & 0xf) != 0)
        return false;
    if (((uintptr_t)des & 0xf) != 0)
        return false;

    for (Uint32 i = 0; i < num_pixels; i++)
    {
        if (src[i] > 1.0f)
            des[i] = 255;
        else if (src[i] < 0.0)
            des[i] = 0;
        else
            des[i] = (Uint8)(src[i] * 255.0f);
    }

    return true;
}

Uint32 ImageCompareFloat(const float* src1, const float* src2, Uint32 num_pixels)
{
    Uint32 num_diff = 0;
    for (Uint32 i = 0; i < num_pixels; i++)
    {
        if (src1[i] != src2[i])
            num_diff++;
    }
    return num_diff;
}

Uint32 ImageCompareUint8(const Uint8* src1, const Uint8* src2, Uint32 num_pixels)
{
    Uint32 num_diff = 0;
    for (Uint32 i = 0; i < num_pixels; i++)
    {
        // Pixels values are allowed to differ by 1 to account for
        // slight variations in FP arithmetic
        if (abs((int)src1[i] - (int)src2[i]) > 1)
            num_diff++;
    }
    return num_diff;
}

void ImageUint8ToFloat(void)
{
    const Uint32 num_pixels = 1024;
    Uint8* src = (Uint8*)_aligned_malloc(num_pixels * sizeof(Uint8), 16);
    float* des1 = (float*)_aligned_malloc(num_pixels * sizeof(float), 16);
    float* des2 = (float*)_aligned_malloc(num_pixels * sizeof(float), 16);

    srand(12);

    for (Uint32 i = 0; i < num_pixels; i++)
        src[i] = (Uint8)(rand() % 256);

    bool rc1 = ImageUnit8ToFloatCpp(des1, src, num_pixels);
    bool rc2 = ImageUint8ToFloat_(des2, src, num_pixels);

    if (!rc1 || !rc2)
    {
        printf("Invalid return code - [%d, %d]\n", rc1, rc2);
        return;
    }

    Uint32 num_diff = ImageCompareFloat(des1, des2, num_pixels);
    printf("\nResults for ImageUint8ToFloat\n");
    printf("  num_diff = %u\n", num_diff);

    _aligned_free(src);
    _aligned_free(des1);
    _aligned_free(des2);
}

void ImageFloatToUint8(void)
{
    const Uint32 num_pixels = 1024;
    float* src = (float*)_aligned_malloc(num_pixels * sizeof(float), 16);
    Uint8* des1 = (Uint8*)_aligned_malloc(num_pixels * sizeof(Uint8), 16);
    Uint8* des2 = (Uint8*)_aligned_malloc(num_pixels * sizeof(Uint8), 16);

    // Initialize the src pixel buffer.  The first few entries in src
    // are set to known values for test purposes.
    src[0] = 0.125f;        src[8] = 0.01f;
    src[1] = 0.75f;         src[9] = 0.99f;
    src[2] = -4.0f;         src[10] = 1.1f;
    src[3] = 3.0f;          src[11] = -1.1f;
    src[4] = 0.0f;          src[12] = 0.99999f;
    src[5] = 1.0f;          src[13] = 0.5f;
    src[6] = -0.01f;        src[14] = -0.0;
    src[7] = +1.01f;        src[15] = .333333f;

    srand(20);
    for (Uint32 i = 16; i < num_pixels; i++)
        src[i] = (float)rand() / RAND_MAX;

    bool rc1 = ImageFloatToUint8Cpp(des1, src, num_pixels);
    bool rc2 = ImageFloatToUint8_(des2, src, num_pixels);

    if (!rc1 || !rc2)
    {
        printf("Invalid return code - [%d, %d]\n", rc1, rc2);
        return;
    }

    Uint32 num_diff = ImageCompareUint8(des1, des2, num_pixels);
    printf("\nResults for ImageFloatToUint8\n");
    printf("  num_diff = %u\n", num_diff);

    _aligned_free(src);
    _aligned_free(des1);
    _aligned_free(des2);
}

int _tmain(int argc, _TCHAR* argv[])
{
    ImageUint8ToFloat();
    ImageFloatToUint8();
    return 0;
}
