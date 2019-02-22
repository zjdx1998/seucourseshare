#pragma once

#include "MiscDefs.h"

// The following structure must match the stucture that's declared
// in the file AvxPackedIntegerPixelClip_.asm.
typedef struct
{
    Uint8* Src;                 // source buffer
    Uint8* Des;                 // destination buffer
    Uint32 NumPixels;           // number of pixels
    Uint32 NumClippedPixels;    // number of clipped pixels
    Uint8 ThreshLo;             // low threshold
    Uint8 ThreshHi;             // high threshold
} PcData;

// Functions defined in AvxPackedIntegerPixelClip.cpp
bool AvxPiPixelClipCpp(PcData* pc_data);

// Functions defined in AvxPackedIntegerPixelClip_.asm
extern "C" bool AvxPiPixelClip_(PcData* pc_data);

// Functions defined in AvxPackedIntegerPixelClipTimed.cpp
void AvxPackedIntegerPixelClipTimed(void);
