#pragma once

#include "MiscDefs.h"

// Functions defined in MmxCalcMinMax.cpp
extern bool MmxCalcMinMaxCpp(Uint8* x, int n, Uint8* x_min, Uint8* x_max);

// Functions defined in MmxCalcMinMaxTimed.cpp
extern void MmxCalcMinMaxTimed(void);

// Functions defined in MmxCalcMinMax_.asm
extern "C" bool MmxCalcMinMax_(Uint8* x, int n, Uint8* x_min, Uint8* x_max);

// Common constants
const int NUM_ELEMENTS = 0x800000;
const int SRAND_SEED = 14;
