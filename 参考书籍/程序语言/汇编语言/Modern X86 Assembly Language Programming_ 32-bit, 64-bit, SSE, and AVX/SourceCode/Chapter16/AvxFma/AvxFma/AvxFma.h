#pragma once
#include "MiscDefs.h"

// These functions are defined in AvxFma.cpp
extern void AvxFmaSmooth5Cpp(float* y, const float*x, Uint32 n, const float* sm5_mask);
extern bool AvxFmaInitX(float* x, Uint32 n);

// These functions are defined in AvxFma_.asm
extern "C" void AvxFmaSmooth5a_(float* y, const float*x, Uint32 n, const float* sm5_mask);
extern "C" void AvxFmaSmooth5b_(float* y, const float*x, Uint32 n, const float* sm5_mask);
extern "C" void AvxFmaSmooth5c_(float* y, const float*x, Uint32 n, const float* sm5_mask);

// These functions are defined in AvxFmaTimed.cpp
extern void AvxFmaTimed(void);
