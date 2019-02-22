#pragma once

#include "ImageBuffer.h"

// Functions defined in SsePackedIntegerHistogram.cpp
extern bool SsePiHistogramCpp(Uint32* histo, const Uint8* pixel_buff, Uint32 num_pixels);

// Functions defined in SsePackedIntegerHistogram_.asm
extern "C" bool SsePiHistogram_(Uint32* histo, const Uint8* pixel_buff, Uint32 num_pixels);

// Functions defined in SsePackedIntegerHistogramTimed.cpp
extern void SsePiHistogramTimed(void);
