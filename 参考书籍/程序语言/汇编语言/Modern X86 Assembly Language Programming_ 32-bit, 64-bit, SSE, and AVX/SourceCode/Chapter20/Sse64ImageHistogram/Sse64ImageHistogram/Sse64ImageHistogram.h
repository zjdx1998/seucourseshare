#pragma once

#include "ImageBuffer.h"

// Functions defined in Sse64ImageHistogram.cpp
extern bool Sse64ImageHistogramCpp(Uint32* histo, const Uint8* pixel_buff, Uint32 num_pixels);

// Functions defined in Sse64ImageHistogram_.asm
extern "C" bool Sse64ImageHistogram_(Uint32* histo, const Uint8* pixel_buff, Uint32 num_pixels);

// Functions defined in Sse64ImageHistogramTimed.cpp
extern void Sse64ImageHistogramTimed(void);
