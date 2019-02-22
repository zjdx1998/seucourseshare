#pragma once

#include "MiscDefs.h"

// Functions defined in MmxCalcMean.cpp
extern bool MmxCalcMeanCpp(const Uint8* x, int n, Uint32* sum_x, double* mean);

// Functions defined in MmxCalcMeanTimed.cpp
extern void MmxCalcMeanTimed(void);

// Functions defined in MmxCalcMean_.asm
extern "C" bool MmxCalcMean_(const Uint8* x, int n, Uint32* sum_x, double* mean);

// Common constants
const int NUM_ELEMENTS = 0x800000;
const int SRAND_SEED = 23;
