#pragma once
#include "ImageBuffer.h"

// Image threshold data structure. This structure must agree with the
// structure that's defined in AvxPackedIntegerThreshold_.asm.
typedef struct
{
    Uint8* PbSrc;               // Source image pixel buffer
    Uint8* PbMask;              // Mask mask pixel buffer
    Uint32 NumPixels;           // Number of source image pixels
    Uint8 Threshold;            // Image threshold value
    Uint8 Pad[3];               // Available for future use
    Uint32 NumMaskedPixels;     // Number of masked pixels
    Uint32 SumMaskedPixels;     // Sum of masked pixels
    double MeanMaskedPixels;    // Mean of masked pixels
} ITD;

// Functions defined in AvxPackedIntegerThreshold.cpp
extern bool AvxPiThresholdCpp(ITD* itd);
extern bool AvxPiCalcMeanCpp(ITD* itd);

// Functions defined in AvxPackedIntegerThreshold_.asm
extern "C" bool AvxPiThreshold_(ITD* itd);
extern "C" bool AvxPiCalcMean_(ITD* itd);

// Functions defined in AvxPackedIntegerThresholdTimed.cpp
extern void AvxPiThresholdTimed(void);

// Miscellaneous constants
const Uint8 TEST_THRESHOLD = 96;
