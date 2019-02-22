#pragma once

// Declarations for functions in NonTemporalStore.cpp
extern bool CompareResults(const float* c1, const float* c2a, const float*c2b, int n, bool pf);
extern bool CalcResultCpp(float* c, const float* a, const float* b, int n);

// Declarations for functions in NonTemporalStore32_.asm
// and NonTemporalStore64_.asm
extern "C" bool CalcResultA_(float* c, const float* a, const float* b, int n);
extern "C" bool CalcResultB_(float* c, const float* a, const float* b, int n);

// Declarations for functions in NonTemporalStoreTimed.cpp
extern void NonTemporalStoreTimed(void);
