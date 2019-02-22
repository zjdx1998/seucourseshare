//
// The code in this file is used to support sample code image management
// and bitmap functions.  It may not be suitable for other purposes.
//

#include "stdafx.h"
#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
#include <malloc.h>
#include <stdexcept>
#include "ImageBuffer.h"

#pragma comment (lib,"Gdiplus.lib")

using namespace std;
using namespace Gdiplus;

const int PixelBufferMallocAlignment = 32;      // Alignment for SSE and AVX

static int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
    unsigned int num = 0;                       // number of image encoders
    unsigned int size = 0;                      // size of the image encoder array in bytes
    ImageCodecInfo* image_codec_info = NULL;

    GetImageEncodersSize(&num, &size);

    if (size == 0)
        return -1;  // Failure

    if ((image_codec_info = (ImageCodecInfo*)(malloc(size))) == NULL)
        return -1;

    GetImageEncoders(num, size, image_codec_info);

    for (unsigned int j = 0; j < num; ++j)
    {
        if (wcscmp(image_codec_info[j].MimeType, format) == 0)
        {
            *pClsid = image_codec_info[j].Clsid;
            free(image_codec_info);
            return j;
        }    
    }

    free(image_codec_info);
    return -1;  // Failure
}

void
ImageBuffer::AllocatePixelBuffer(int height, int width, int bytes_per_pixel, PixelType type)
{
    Height = height;
    Width = width;
    BytesPerPixel = bytes_per_pixel;
    PixelBufferSize = height * width * bytes_per_pixel;
    Type = type;
    PixelBuffer = _aligned_malloc(PixelBufferSize, PixelBufferMallocAlignment);
}

ImageBuffer::ImageBuffer(const wchar_t* bitmap_filename)
{
    ULONG_PTR GdiToken;
    GdiplusStartupInput GdiInput;
    Status st = GdiplusStartup(&GdiToken, &GdiInput, NULL);
    Bitmap* bm = new Bitmap(bitmap_filename, FALSE);
    PixelFormat pixel_format = bm->GetPixelFormat();

    if (pixel_format == PixelFormat8bppIndexed)
    {
        AllocatePixelBuffer(bm->GetHeight(), bm->GetWidth(), 1, PixelType::Gray8);
        Uint8* pb = (Uint8*)PixelBuffer;

        for (int i = 0; i < Height; i++)
        {
            for (int j = 0; j < Width; j++)
            {
                Gdiplus::Color c;

                if (bm->GetPixel(j, i, &c) != Gdiplus::Ok)
                    throw new runtime_error("ImageBuffer::ImageBuffer - Bitmap::GetPixel() error occurred");

                *pb++ = c.GetGreen();
            }
        }
    }
    else if (pixel_format == PixelFormat24bppRGB)
    {
        AllocatePixelBuffer(bm->GetHeight(), bm->GetWidth(), sizeof(RGB32), PixelType::Rgb32);
        RGB32* pb = (RGB32*)PixelBuffer;

        for (int i = 0; i < Height; i++)
        {
            for (int j = 0; j < Width; j++)
            {
                RGB32 rgb;
                Gdiplus::Color c;

                if (bm->GetPixel(j, i, &c) != Gdiplus::Ok)
                    throw new runtime_error("ImageBuffer::ImageBuffer - Bitmap::GetPixel() error occurred");

                rgb.R = c.GetRed();
                rgb.G = c.GetGreen();
                rgb.B = c.GetBlue();
                rgb.A = 0;

                *pb++ = rgb;
            }
        }
    }
    else
        throw new runtime_error("ImageBuffer::ImageBuffer(const wchar_t* bitmap_filename) - Unsupported pixel format");

    delete bm;
    GdiplusShutdown(GdiToken);
}

ImageBuffer::ImageBuffer(int height, int width, int bytes_per_pixel, PixelType type)
{
    AllocatePixelBuffer(height, width, bytes_per_pixel, type);
}

ImageBuffer::ImageBuffer(const ImageBuffer& image_buffer)
{
    AllocatePixelBuffer(image_buffer.Height, image_buffer.Width, image_buffer.BytesPerPixel, image_buffer.Type);
    memcpy(PixelBuffer, image_buffer.PixelBuffer, PixelBufferSize);
}

ImageBuffer::ImageBuffer(const ImageBuffer& image_buffer, bool copy_data)
{
    AllocatePixelBuffer(image_buffer.Height, image_buffer.Width, image_buffer.BytesPerPixel, image_buffer.Type);

    if (copy_data)
        memcpy(PixelBuffer, image_buffer.PixelBuffer, PixelBufferSize);
}

ImageBuffer::~ImageBuffer()
{
    _aligned_free(PixelBuffer);
}

void
ImageBuffer::SaveToBitmapFile(const wchar_t* filename)
{
    ULONG_PTR GdiToken;
    GdiplusStartupInput GdiInput;
    Status st = GdiplusStartup(&GdiToken, &GdiInput, NULL);
    Bitmap* bm = NULL;
    BitmapData bitmapData;
    Rect rect(0, 0, Width, Height);

    if (Type == PixelType::Gray8)
    {
        if (Width % 4 == 0)
        {
            bm = new Bitmap(Width, Height, PixelFormat8bppIndexed);
            bm->LockBits(&rect, ImageLockModeWrite, PixelFormat8bppIndexed, &bitmapData);

            Uint8* pb_src = (Uint8*)PixelBuffer;
            Uint8* pb_des = (Uint8*)bitmapData.Scan0;
            memcpy(pb_des, pb_src, Height * Width);
            bm->UnlockBits(&bitmapData);
        }
        else
            throw new runtime_error("ImageBuffer::SaveToBitmapFile(const wchar_t* filename) - Bitmap width must be evenly divisible by 4");
    }
    else if (Type == PixelType::Rgb32)
    {
        bm = new Bitmap(Width, Height, PixelFormat24bppRGB);

        RGB32* pb_src = (RGB32*)PixelBuffer;
        Gdiplus::Color c;

        for (int i = 0; i < Height; i++)
        {
            for (int j = 0; j < Width; j++)
            {
                RGB32 rgb = *pb_src++;
                ARGB argb = Gdiplus::Color::MakeARGB(0, rgb.R, rgb.G, rgb.B);

                c.SetValue(argb);

                if (bm->SetPixel(j, i, c) != Gdiplus::Ok)
                    throw new runtime_error("ImageBuffer::SaveToBitmapFile(const wchar_t* filename) - SetPixel() error occurred");
            }
        }
    }
    else
        throw new runtime_error("ImageBuffer::SaveToBitmapFile(const wchar_t* filename) - Unsupported pixel format");

    CLSID pngClsid;

    GetEncoderClsid(L"image/bmp", &pngClsid);
    st = bm->Save(filename, &pngClsid, NULL);
    delete bm;

    GdiplusShutdown(GdiToken);
}

void ImageBuffer::AllocRgbArrays(Uint8** r, Uint8** g, Uint8** b, size_t alignment)
{
    if (Type != PixelType::Rgb32)
        throw new runtime_error("ImageBuffer::AllocRgbArrays(size_t alignment) - Invalid 'Type' value");

    *r = (Uint8*)_aligned_malloc(Width * Height * sizeof(Uint8), alignment);
    *g = (Uint8*)_aligned_malloc(Width * Height * sizeof(Uint8), alignment);
    *b = (Uint8*)_aligned_malloc(Width * Height * sizeof(Uint8), alignment);
}

void ImageBuffer::FreeRgbArrays(Uint8* r, Uint8* g, Uint8* b)
{
    if (Type != PixelType::Rgb32)
        throw new runtime_error("ImageBuffer::FreeRgbArrays(size_t alignment) - Invalid 'Type' value");

    _aligned_free(r);
    _aligned_free(g);
    _aligned_free(b);
}

void ImageBuffer::GetRgbPixelData(Uint8* r, Uint8* g, Uint8* b)
{
    if (Type != PixelType::Rgb32)
        throw new runtime_error("ImageBuffer::GetRgbPixelData(Uint8* r, Uint8* g, Uint8* b) - Invalid 'Type' value");

    RGB32* pb = (RGB32*)PixelBuffer;

    for (int i = 0; i < Height * Width; i++, pb++)
    {
        *r++ = pb->R;
        *g++ = pb->G;
        *b++ = pb->B;
    }
}

void ImageBuffer::SetRgbPixelData(const Uint8* r, const Uint8* g, const Uint8* b)
{
    if (Type != PixelType::Rgb32)
        throw new runtime_error("ImageBuffer::SetRgbPixelData(const Uint8* r, const Uint8* g, const Uint8* b) - Invalid 'Type' value");

    RGB32* pb = (RGB32*)PixelBuffer;

    for (int i = 0; i < Height * Width; i++, pb++)
    {
        pb->R = *r++;
        pb->G = *g++;
        pb->B = *b++;
    }
}
