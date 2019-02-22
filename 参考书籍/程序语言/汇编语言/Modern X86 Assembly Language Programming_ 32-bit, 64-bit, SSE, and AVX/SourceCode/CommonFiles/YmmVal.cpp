#include "stdafx.h"
#include "YmmVal.h"
#include <stdio.h>

char* YmmVal::ToString_i8(char* s, size_t len, bool upper_half)
{
    const char* f = "%4d %4d %4d %4d %4d %4d %4d %4d | %4d %4d %4d %4d %4d %4d %4d %4d";

    if (!upper_half)
        sprintf_s(s, len, f,
        i8[0],  i8[1],  i8[2],  i8[3],
        i8[4],  i8[5],  i8[6],  i8[7],
        i8[8],  i8[9],  i8[10], i8[11],
        i8[12], i8[13], i8[14], i8[15]);
    else
        sprintf_s(s, len, f,
        i8[16], i8[17], i8[18], i8[19],
        i8[20], i8[21], i8[22], i8[23],
        i8[24], i8[25], i8[26], i8[27],
        i8[28], i8[29], i8[30], i8[31]);

    return s;
}

char* YmmVal::ToString_i16(char* s, size_t len, bool upper_half)
{
    const char* f = "%8d %8d %8d %8d | %8d %8d %8d %8d";

    if (!upper_half)
        sprintf_s(s, len, f,
            i16[0], i16[1], i16[2], i16[3],
            i16[4], i16[5], i16[6], i16[7]);
    else
        sprintf_s(s, len, f,
            i16[8], i16[9], i16[10], i16[11],
            i16[12], i16[13], i16[14], i16[15]);

    return s;
}

char* YmmVal::ToString_i32(char* s, size_t len, bool upper_half)
{
    const char* f = "%12d %12d | %12d %12d";

    if (!upper_half)
        sprintf_s(s, len, f, i32[0], i32[1], i32[2], i32[3]);
    else
        sprintf_s(s, len, f, i32[4], i32[5], i32[6], i32[7]);

    return s;
}

char* YmmVal::ToString_i64(char* s, size_t len, bool upper_half)
{
    const char* f = "%16lld | %16lld";

    if (!upper_half)
        sprintf_s(s, len, f, i64[0], i64[1]);
    else
        sprintf_s(s, len, f, i64[2], i64[3]);

    return s;
}

char* YmmVal::ToString_u8(char* s, size_t len, bool upper_half)
{
    const char* f = "%4u %4u %4u %4u %4u %4u %4u %4u | %4u %4u %4u %4u %4u %4u %4u %4u";

    if (!upper_half)
        sprintf_s(s, len, f,
            u8[0],  u8[1],  u8[2],  u8[3],
            u8[4],  u8[5],  u8[6],  u8[7],
            u8[8],  u8[9],  u8[10], u8[11],
            u8[12], u8[13], u8[14], u8[15]);
    else
        sprintf_s(s, len, f,
            u8[16], u8[17], u8[18], u8[19],
            u8[20], u8[21], u8[22], u8[23],
            u8[24], u8[24], u8[26], u8[27],
            u8[28], u8[29], u8[30], u8[31]);

    return s;
}

char* YmmVal::ToString_u16(char* s, size_t len, bool upper_half)
{
    const char* f = "%8u %8u %8u %8u | %8u %8u %8u %8u";

    if (!upper_half)
        sprintf_s(s, len, f,
            u16[0], u16[1], u16[2], u16[3],
            u16[4], u16[5], u16[6], u16[7]);
    else
        sprintf_s(s, len, f,
            u16[8],  u16[9],  u16[10], u16[11],
            u16[12], u16[13], u16[14], u16[15]);

    return s;
}

char* YmmVal::ToString_u32(char* s, size_t len, bool upper_half)
{
    const char* f = "%12u %12u | %12u %12u";

    if (!upper_half)
        sprintf_s(s, len, f, u32[0], u32[1], u32[2], u32[3]);
    else
        sprintf_s(s, len, f, u32[4], u32[5], u32[6], u32[7]);

    return s;
}

char* YmmVal::ToString_u64(char* s, size_t len, bool upper_half)
{
    const char* f = "%16llu | %16llu";

    if (!upper_half)
        sprintf_s(s, len, f, u64[0], u64[1]);
    else
        sprintf_s(s, len, f, u64[2], u64[3]);

    return s;
}

char* YmmVal::ToString_x8(char* s, size_t len, bool upper_half)
{
    const char* f = "%02X %02X %02X %02X %02X %02X %02X %02X | %02X %02X %02X %02X %02X %02X %02X %02X";

    if (!upper_half)
        sprintf_s(s, len, f,
            u8[0],  u8[1],  u8[2],  u8[3],
            u8[4],  u8[5],  u8[6],  u8[7],
            u8[8],  u8[9],  u8[10], u8[11],
            u8[12], u8[13], u8[14], u8[15]);
    else
        sprintf_s(s, len, f,
            u8[16], u8[17], u8[18], u8[19],
            u8[20], u8[21], u8[22], u8[23],
            u8[24], u8[24], u8[26], u8[27],
            u8[28], u8[29], u8[30], u8[31]);

    return s;
}

char* YmmVal::ToString_x16(char* s, size_t len, bool upper_half)
{
    const char* f = "%04X %04X %04X %04X | %04X %04X %04X %04X";
    if (!upper_half)
        sprintf_s(s, len, f,
            u16[0], u16[1], u16[2], u16[3],
            u16[4], u16[5], u16[6], u16[7]);
    else
        sprintf_s(s, len, f,
            u16[8],  u16[9],  u16[10], u16[11],
            u16[12], u16[13], u16[14], u16[15]);

    return s;
}

char* YmmVal::ToString_x32(char* s, size_t len, bool upper_half)
{
    const char* f = "%08X %08X | %08X %08X";

    if (!upper_half)
        sprintf_s(s, len, f, u32[0], u32[1], u32[2], u32[3]);
    else
        sprintf_s(s, len, f, u32[4], u32[5], u32[6], u32[7]);

    return s;
}

char* YmmVal::ToString_x64(char* s, size_t len, bool upper_half)
{
    const char* f = "%016llX | %016llX";

    if (!upper_half)
        sprintf_s(s, len, f, u64[0], u64[1]);
    else
        sprintf_s(s, len, f, u64[2], u64[3]);

    return s;
}

char* YmmVal::ToString_r32(char* s, size_t len, bool upper_half)
{
    const char* f = "%12.6f %12.6f | %12.6f %12.6f";

    if (!upper_half)
        sprintf_s(s, len, f, r32[0], r32[1], r32[2], r32[3]);
    else
        sprintf_s(s, len, f, r32[4], r32[5], r32[6], r32[7]);

    return s;
}

char* YmmVal::ToString_r64(char* s, size_t len, bool upper_half)
{
    const char* f = "%24.12lf  | %24.12lf";

    if (!upper_half)
        sprintf_s(s, len, f, r64[0], r64[1]);
    else
        sprintf_s(s, len, f, r64[2], r64[3]);

    return s;
}
