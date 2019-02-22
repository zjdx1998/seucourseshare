#pragma once
#include "MiscDefs.h"

// Simple vector structure
typedef struct
{
    float X;        // Vector X component
    float Y;        // Vector Y component
    float Z;        // Vector Z component
    float Pad;      // Pad for 16 byte structure size
} Vector;

// Vector structure of arrays
typedef struct
{
    float* X;       // Pointer to X components
    float* Y;       // Pointer to Y copmonents
    float* Z;       // Pointer to Z components
} VectorSoA;

extern void Sse64VectorCrossProdTimed(void);
extern "C" bool Sse64VectorCrossProd1_(Vector* c, const Vector* a, const Vector* b, Uint32 num_vectors);
extern "C" bool Sse64VectorCrossProd2_(VectorSoA* c, const VectorSoA* a, const VectorSoA* b, Uint32 num_vectors);
