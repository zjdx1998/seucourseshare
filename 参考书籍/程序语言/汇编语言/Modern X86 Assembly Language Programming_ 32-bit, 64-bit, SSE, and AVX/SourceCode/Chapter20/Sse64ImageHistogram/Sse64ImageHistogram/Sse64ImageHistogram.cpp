#include "stdafx.h"
#include "Sse64ImageHistogram.h"
#include <string.h>
#include <malloc.h>

extern "C" Uint32 NUM_PIXELS_MAX = 16777216;

bool Sse64ImageHistogramCpp(Uint32* histo, const Uint8* pixel_buff, Uint32 num_pixels)
{
    // Make sure num_pixels is valid
    if ((num_pixels == 0) || (num_pixels > NUM_PIXELS_MAX))
        return false;
    if (num_pixels % 32 != 0)
        return false;

    // Make sure histo is aligned to a 16-byte boundary
    if (((uintptr_t)histo & 0xf) != 0)
        return false;

    // Make sure pixel_buff is aligned to a 16-byte boundary
    if (((uintptr_t)pixel_buff & 0xf) != 0)
        return false;

    // Build the histogram
    memset(histo, 0, 256 * sizeof(Uint32));

    for (Uint32 i = 0; i < num_pixels; i++)
        histo[pixel_buff[i]]++;

    return true;
}

void Sse64ImageHistogram(void)
{
    const wchar_t* image_fn = L"..\\..\\..\\DataFiles\\TestImage1.bmp";
    const char* csv_fn = "__TestImage1_Histograms.csv";

    ImageBuffer ib(image_fn);
    Uint32 num_pixels = ib.GetNumPixels();
    Uint8* pixel_buff = (Uint8*)ib.GetPixelBuffer();
    Uint32* histo1 = (Uint32*)_aligned_malloc(256 * sizeof(Uint32), 16);
    Uint32* histo2 = (Uint32*)_aligned_malloc(256 * sizeof(Uint32), 16);
    bool rc1, rc2;

    rc1 = Sse64ImageHistogramCpp(histo1, pixel_buff, num_pixels);
    rc2 = Sse64ImageHistogram_(histo2, pixel_buff, num_pixels);

    printf("Results for Sse64ImageHistogram()\n");

    if (!rc1 || !rc2)
    {
        printf("  Bad return code: rc1=%d, rc2=%d\n", rc1, rc2);
        return;
    }

    FILE* fp;
    bool compare_error = false;

    if (fopen_s(&fp, csv_fn, "wt") != 0)
        printf("  File open error: %s\n", csv_fn);
    else
    {
        for (Uint32 i = 0; i < 256; i++)
        {
            fprintf(fp, "%u, %u, %u\n", i, histo1[i], histo2[i]);

            if (histo1[i] != histo2[i])
            {
                printf("  Histogram compare error at index %u\n", i);
                printf("    counts: [%u, %u]\n", histo1[i], histo2[i]);
                compare_error = true;
            }
        }

        if (!compare_error)
            printf("  Histograms are identical\n");

        fclose(fp);
    }
}

int _tmain(int argc, _TCHAR* argv[])
{
    try
    {
        Sse64ImageHistogram();
        Sse64ImageHistogramTimed();
    }

    catch (...)
    {
        printf("Unexpected exception has occurred!\n");
        printf("File: %s (_tmain)\n", __FILE__);
    }

    return 0;
}
