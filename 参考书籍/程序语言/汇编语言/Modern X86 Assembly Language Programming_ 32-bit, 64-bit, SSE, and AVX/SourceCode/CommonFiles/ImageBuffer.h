//
// The code in this file is used to support sample code image management
// and bitmap functions.  It may not be suitable for other purposes.
//

#pragma once
#include "MiscDefs.h"

typedef struct
{
    Uint8 R;
    Uint8 G;
    Uint8 B;
    Uint8 A;

} RGB32;

typedef enum
{
    Undefined,
    Gray8,
    Rgb32

} PixelType;

class ImageBuffer
{
    int Height;
    int Width;
    int BytesPerPixel;
    int PixelBufferSize;
    void* PixelBuffer;
    PixelType Type;

    void AllocatePixelBuffer(int height, int width, int bytes_per_pixel, PixelType type);

public:

    int GetHeight()                 { return Height; }
    int GetWidth()                  { return Width; }
    int GetNumPixels()              { return Height * Width; }
    int GetBytesPerPixel()          { return BytesPerPixel; }
    int GetPixelBufferSize()        { return PixelBufferSize; }
    PixelType GetPixelBufferType()  { return Type; }
    void* GetPixelBuffer()          { return PixelBuffer; }

    ImageBuffer(const wchar_t* filename);
    ImageBuffer(int height, int width, int bytes_per_pixel, PixelType type);
    ImageBuffer(const ImageBuffer& image_buffer);
    ImageBuffer(const ImageBuffer& image_buffer, bool copy_data);
    ~ImageBuffer(void);

    void SaveToBitmapFile(const wchar_t* filename);

    void AllocRgbArrays(Uint8** r, Uint8** g, Uint8** b, size_t alignment);
    void FreeRgbArrays(Uint8* r, Uint8* g, Uint8* b);
    void GetRgbPixelData(Uint8* r, Uint8* g, Uint8* b);
    void SetRgbPixelData(const Uint8* r, const Uint8* g, const Uint8* b);
};
