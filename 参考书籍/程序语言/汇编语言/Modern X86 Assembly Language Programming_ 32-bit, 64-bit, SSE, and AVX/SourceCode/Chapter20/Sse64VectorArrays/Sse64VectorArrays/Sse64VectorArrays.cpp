#include "stdafx.h"
#include "Sse64VectorArrays.h"
#include <stdlib.h>

void Sse64VectorCrossProd(void)
{
    const Uint32 num_vectors = 8;
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
        a1[i].Pad = b1[i].Pad = 0;
    }

    Sse64VectorCrossProd1_(c1, a1, b1, num_vectors);
    Sse64VectorCrossProd2_(&c2, &a2, &b2, num_vectors);

    bool error = false;
    printf("Results for Sse64VectorCrossProd()\n\n");

    for (Uint32 i = 0; i < num_vectors && !error; i++)
    {
        const char* fs = "[%8.1f %8.1f %8.1f]\n";

        printf("Vector cross product %d\n", i);

        printf("  a1/a2: ");
        printf(fs, a1[i].X, a1[i].Y, a1[i].Z);
        printf("  b1/b2: ");
        printf(fs, b1[i].X, b1[i].Y, b1[i].Z);
        printf("  c1:    ");
        printf(fs, c1[i].X, c1[i].Y, c1[i].Z);
        printf("  c2:    ");
        printf(fs, c2.X[i], c2.Y[i], c2.Z[i]);
        printf("\n");

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

    _aligned_free(a1);   _aligned_free(b1);   _aligned_free(c1);
    _aligned_free(a2.X); _aligned_free(a2.Y); _aligned_free(a2.Z);
    _aligned_free(b2.X); _aligned_free(b2.Y); _aligned_free(b2.Z);
    _aligned_free(c2.X); _aligned_free(c2.Y); _aligned_free(c2.Z);
}

int _tmain(int argc, _TCHAR* argv[])
{
    Sse64VectorCrossProd();
    Sse64VectorCrossProdTimed();
}
