#include "stdafx.h"
#include "MiscDefs.h"
#include <stdlib.h>
#include <malloc.h>

extern "C" bool Avx64CalcRgbMinMax_(Uint8* rgb[3], Uint32 num_pixels, Uint8 min_vals[3], Uint8 max_vals[3]);

bool Avx64CalcRgbMinMaxCpp(Uint8* rgb[3], Uint32 num_pixels, Uint8 min_vals[3], Uint8 max_vals[3])
{
    // Make sure num_pixels is valid
    if ((num_pixels == 0) || (num_pixels % 32 != 0))
        return false;

    // Make sure the color planes are properly aligned
    for (Uint32 i = 0; i < 3; i++)
    {
        if (((uintptr_t)rgb[i] & 0x1f) != 0)
            return false;
    }

    // Find the min and max of each color plane
    for (Uint32 i = 0; i < 3; i++)
    {
        min_vals[i] = 255;   max_vals[i] = 0;

        for (Uint32 j = 0; j < num_pixels; j++)
        {
            if (rgb[i][j] < min_vals[i])
                min_vals[i] = rgb[i][j];
            else if (rgb[i][j] > max_vals[i])
                max_vals[i] = rgb[i][j];
        }
    }

    return true;
}

int _tmain(int argc, _TCHAR* argv[])
{
    const Uint32 n = 1024;
    Uint8* rgb[3];

    rgb[0] = (Uint8*)_aligned_malloc(n * sizeof(Uint8), 32);
    rgb[1] = (Uint8*)_aligned_malloc(n * sizeof(Uint8), 32);
    rgb[2] = (Uint8*)_aligned_malloc(n * sizeof(Uint8), 32);

    for (Uint32 i = 0; i < n; i++)
    {
        rgb[0][i] = 5 + rand() % 245;
        rgb[1][i] = 5 + rand() % 245;
        rgb[2][i] = 5 + rand() % 245;
    }

    // Initialize known min & max values for validation purposes
    rgb[0][n / 4] = 4;   rgb[1][n / 2] = 1;       rgb[2][3 * n / 4] = 3;
    rgb[0][n / 3] = 254; rgb[1][2 * n / 5] = 251; rgb[2][n - 1] = 252;

    Uint8 min_vals1[3], max_vals1[3];
    Uint8 min_vals2[3], max_vals2[3];

    Avx64CalcRgbMinMaxCpp(rgb, n, min_vals1, max_vals1);
    Avx64CalcRgbMinMax_(rgb, n, min_vals2, max_vals2);

    printf("Results for Avx64CalcRgbMinMax\n\n");
    printf("             R   G   B\n");
    printf("----------------------\n");
    printf("min_vals1: %3d %3d %3d\n", min_vals1[0], min_vals1[1], min_vals1[2]);
    printf("min_vals2: %3d %3d %3d\n", min_vals2[0], min_vals2[1], min_vals2[2]);
    printf("\n");
    printf("max_vals1: %3d %3d %3d\n", max_vals1[0], max_vals1[1], max_vals1[2]);
    printf("max_vals2: %3d %3d %3d\n", max_vals2[0], max_vals2[1], max_vals2[2]);

    _aligned_free(rgb[0]);
    _aligned_free(rgb[1]);
    _aligned_free(rgb[2]);
    return 0;
}
