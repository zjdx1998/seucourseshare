#include "stdafx.h"
#include "Sse64VectorArrays.h"
#include "ThreadTimer.h"
#include <stdlib.h>

void Sse64VectorCrossProdTimed(void)
{
    ThreadTimer::SetThreadAffinityMask();
    const int num_it = 500;
    const int num_alg = 2;
    const double et_scale = 1.0e6;
    double et[num_it][num_alg];
    ThreadTimer tt;

    const Uint32 num_vectors = 50000;
    const size_t vsize1 = num_vectors * sizeof(Vector);
    const size_t vsize2 = num_vectors * sizeof(float);
    Vector* a1 = (Vector*)_aligned_malloc(vsize1, 16);
    Vector* b1 = (Vector*)_aligned_malloc(vsize1, 16);
    Vector* c1 = (Vector*)_aligned_malloc(vsize1, 16);
    VectorSoA a2, b2, c2;

    a2.X = (float*)_aligned_malloc(vsize2, 16);
    a2.Y = (float*)_aligned_malloc(vsize2, 16);
    a2.Z = (float*)_aligned_malloc(vsize2, 16);
    b2.X = (float*)_aligned_malloc(vsize2, 16);
    b2.Y = (float*)_aligned_malloc(vsize2, 16);
    b2.Z = (float*)_aligned_malloc(vsize2, 16);
    c2.X = (float*)_aligned_malloc(vsize2, 16);
    c2.Y = (float*)_aligned_malloc(vsize2, 16);
    c2.Z = (float*)_aligned_malloc(vsize2, 16);

    srand(103);
    for (Uint32 i = 0; i < num_vectors; i++)
    {
        float a_x = (float)(rand() % 100);
        float a_y = (float)(rand() % 100);
        float a_z = (float)(rand() % 100);
        float b_x = (float)(rand() % 100);
        float b_y = (float)(rand() % 100);
        float b_z = (float)(rand() % 100);

        a1[i].X = a2.X[i] = a_x;
        a1[i].Y = a2.Y[i] = a_y;
        a1[i].Z = a2.Z[i] = a_z;
        b1[i].X = b2.X[i] = b_x;
        b1[i].Y = b2.Y[i] = b_y;
        b1[i].Z = b2.Z[i] = b_z;
    }

    for (int i = 0; i < num_it; i++)
    {
        tt.Start();
        Sse64VectorCrossProd1_(c1, a1, b1, num_vectors);
        tt.Stop();
        et[i][0] = tt.GetElapsedTime() * et_scale;
    }

    for (int i = 0; i < num_it; i++)
    {
        tt.Start();
        Sse64VectorCrossProd2_(&c2, &a2, &b2, num_vectors);
        tt.Stop();
        et[i][1] = tt.GetElapsedTime() * et_scale;
    }

    bool error = false;

    for (Uint32 i = 0; i < num_vectors && !error; i++)
    {
        bool error_x = c1[i].X != c2.X[i];
        bool error_y = c1[i].Y != c2.Y[i];
        bool error_z = c1[i].Z != c2.Z[i];

        if (error_x || error_y || error_z)
        {
            printf("Compare error at index %d\n", i);
            printf("  %d, %d, %d\n", error_x, error_y, error_z);
            error = true;
        }
    }

    const char* fn = "__Sse64VectorCrossProdTimed.csv";
    ThreadTimer::SaveElapsedTimeMatrix(fn, (double*)et, num_it, num_alg);
    printf("\nBenchmark times saved to file %s\n", fn);
}
