#include "stdafx.h"
#include "AvxFma.h"
#include <stdio.h>
#include <malloc.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>

bool AvxFmaInitX(float* x, Uint32 n)
{
    const float degtorad = (float)(M_PI / 180.0);
    const float t_start = 0;
    const float t_step = 0.002f;
    const Uint32 m = 3;
    const float amp[m] = {1.0f, 0.80f, 1.20f};
    const float freq[m] = {5.0f, 10.0f, 15.0f};
    const float phase[m] = {0.0f, 45.0f, 90.0f};
    float t = t_start;

    srand(97);

    for (Uint32 i = 0; i < n; i++, t += t_step)
    {
        float x_total = 0;

        for (Uint32 j = 0; j < m; j++)
        {
            float omega = 2.0f * (float)M_PI * freq[j];
            float x_temp1 = amp[j] * sin(omega * t + phase[j] * degtorad);
            float noise = (float)((rand() % 300) - 150) / 10.0f;
            float x_temp2 = x_temp1 + x_temp1 * noise / 100.0f;

            x_total += x_temp2;
        }

        x[i] = x_total;
    }

    return true;
}

void AvxFmaSmooth5Cpp(float* y, const float*x, Uint32 n, const float* sm5_mask)
{
    for (Uint32 i = 2; i < n - 2; i++)
    {
        float sum = 0;

        sum += x[i - 2] * sm5_mask[0];
        sum += x[i - 1] * sm5_mask[1];
        sum += x[i + 0] * sm5_mask[2];
        sum += x[i + 1] * sm5_mask[3];
        sum += x[i + 2] * sm5_mask[4];
        y[i] = sum;
    }
}

void AvxFma(void)
{
    const Uint32 n = 512;
    float* x = (float*)_aligned_malloc(n * sizeof(float), 32);
    float* y_cpp = (float*)_aligned_malloc(n * sizeof(float), 32);
    float* y_a = (float*)_aligned_malloc(n * sizeof(float), 32);
    float* y_b = (float*)_aligned_malloc(n * sizeof(float), 32);
    float* y_c = (float*)_aligned_malloc(n * sizeof(float), 32);
    const float sm5_mask[] = { 0.0625f, 0.25f, 0.375f, 0.25f, 0.0625f };

    printf("Results for AvxFma\n");

    if (!AvxFmaInitX(x, n))
    {
        printf("Data initialization failed\n");
        return;
    }

    AvxFmaSmooth5Cpp(y_cpp, x, n, sm5_mask);
    AvxFmaSmooth5a_(y_a, x, n, sm5_mask);
    AvxFmaSmooth5b_(y_b, x, n, sm5_mask);
    AvxFmaSmooth5b_(y_c, x, n, sm5_mask);

    FILE* fp;
    const char* fn = "__AvxFmaRawData.csv";

    if (fopen_s(&fp, fn, "wt") != 0)
    {
        printf("File open failed (%s)\n", fn);
        return;
    }

    fprintf(fp, "i, x, y_cpp, y_a, y_b, y_c, ");
    fprintf(fp, "diff_ab, diff_ac, diff_bc\n");

    Uint32 num_diff_ab = 0, num_diff_ac = 0, num_diff_bc = 0;

    for (Uint32 i = 2; i < n - 2; i++)
    {
        bool diff_ab = false, diff_ac = false, diff_bc = false;

        if (y_a[i] != y_b[i])
        {
            diff_ab = true;
            num_diff_ab++;
        }

        if (y_a[i] != y_c[i])
        {
            diff_ac = true;
            num_diff_ac++;
        }

        if (y_b[i] != y_c[i])
        {
            diff_bc = true;
            num_diff_bc++;
        }

        const char* fs1 = "%15.8f, ";
        fprintf(fp, "%4d, ", i);
        fprintf(fp, fs1, x[i]);
        fprintf(fp, fs1, y_cpp[i]);
        fprintf(fp, fs1, y_a[i]);
        fprintf(fp, fs1, y_b[i]);
        fprintf(fp, fs1, y_c[i]);
        fprintf(fp, "%d, %d, %d, ", diff_ab, diff_ac, diff_bc);
        fprintf(fp, "\n");
    }

    fclose(fp);
    printf("\nRaw data saved to file %s\n", fn);
    printf("\nNumber of data point differences between\n");
    printf("  y_a and y_b: %u\n", num_diff_ab);
    printf("  y_a and y_c: %u\n", num_diff_ac);
    printf("  y_b and y_c: %u\n", num_diff_bc);

    _aligned_free(x);
    _aligned_free(y_cpp);
    _aligned_free(y_a);
    _aligned_free(y_b);
    _aligned_free(y_c);
}

int _tmain(int argc, _TCHAR* argv[])
{
    try
    {
        AvxFma();
        AvxFmaTimed();
    }

    catch (...)
    {
        printf("Unexpected exception has occurred!\n");
        printf("File: %s (_tmain)\n", __FILE__);
    }

    return 0;
}
