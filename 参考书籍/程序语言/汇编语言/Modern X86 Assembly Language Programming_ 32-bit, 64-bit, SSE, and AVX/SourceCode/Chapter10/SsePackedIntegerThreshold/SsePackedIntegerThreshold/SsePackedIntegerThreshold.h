#pragma once
#include "ImageBuffer.h"

// Image threshold data structure. This structure must agree with the
// structure that's defined in SsePackedIntegerThreshold_.asm.
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

// Functions defined in SsePackedIntegerThreshold.cpp
extern bool SsePiThresholdCpp(ITD* itd);
extern bool SsePiCalcMeanCpp(ITD* itd);

// Functions defined in SsePackedIntegerThreshold_.asm
extern "C" bool SsePiThreshold_(ITD* itd);
extern "C" bool SsePiCalcMean_(ITD* itd);

// Functions defined in SsePackedIntegerThresholdTimed.cpp
extern void SsePiThresholdTimed(void);

// Miscellaneous constants
const Uint8 TEST_THRESHOLD = 96;
