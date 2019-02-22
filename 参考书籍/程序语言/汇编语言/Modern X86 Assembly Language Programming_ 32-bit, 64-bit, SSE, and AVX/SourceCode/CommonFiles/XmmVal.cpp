#include "stdafx.h"
#include "XmmVal.h"
#include <stdio.h>

char* XmmVal::ToString_i8(char* s, size_t len)
{
    sprintf_s(s, len, "%4d %4d %4d %4d %4d %4d %4d %4d | %4d %4d %4d %4d %4d %4d %4d %4d",
        i8[0], i8[1], i8[2], i8[3],
        i8[4], i8[5], i8[6], i8[7],
        i8[8], i8[9], i8[10], i8[11],
        i8[12], i8[13], i8[14], i8[15]);
    return s;
}

char* XmmVal::ToString_i16(char* s, size_t len)
{
    sprintf_s(s, len, "%8d %8d %8d %8d | %8d %8d %8d %8d",
        i16[0], i16[1], i16[2], i16[3],
        i16[4], i16[5], i16[6], i16[7]);
    return s;
}

char* XmmVal::ToString_i32(char* s, size_t len)
{
    sprintf_s(s, len, "%12d %12d | %12d %12d",
        i32[0], i32[1], i32[2], i32[3]);
    return s;
}

char* XmmVal::ToString_i64(char* s, size_t len)
{
    sprintf_s(s, len, "%25lld | %25lld", i64[0], i64[1]);
    return s;
}

char* XmmVal::ToString_u8(char* s, size_t len)
{
    sprintf_s(s, len, "%4u %4u %4u %4u %4u %4u %4u %4u | %4u %4u %4u %4u %4u %4u %4u %4u",
        u8[0], u8[1], u8[2], u8[3],
        u8[4], u8[5], u8[6], u8[7],
        u8[8], u8[9], u8[10], u8[11],
        u8[12], u8[13], u8[14], u8[15]);
    return s;
}

char* XmmVal::ToString_u16(char* s, size_t len)
{
    sprintf_s(s, len, "%8u %8u %8u %8u | %8u %8u %8u %8u",
        u16[0], u16[1], u16[2], u16[3],
        u16[4], u16[5], u16[6], u16[7]);
    return s;
}

char* XmmVal::ToString_u32(char* s, size_t len)
{
    sprintf_s(s, len, "%12u %12u | %12u %12u",
        u32[0], u32[1], u32[2], u32[3]);
    return s;
}

char* XmmVal::ToString_u64(char* s, size_t len)
{
    sprintf_s(s, len, "%25llu | %25llu", u64[0], u64[1]);
    return s;
}

char* XmmVal::ToString_x8(char* s, size_t len)
{
    sprintf_s(s, len, "%02X %02X %02X %02X %02X %02X %02X %02X | %02X %02X %02X %02X %02X %02X %02X %02X",
        u8[0], u8[1], u8[2], u8[3],
        u8[4], u8[5], u8[6], u8[7],
        u8[8], u8[9], u8[10], u8[11],
        u8[12], u8[13], u8[14], u8[15]);
    return s;
}

char* XmmVal::ToString_x16(char* s, size_t len)
{
    sprintf_s(s, len, "%04X %04X %04X %04X | %04X %04X %04X %04X",
        u16[0], u16[1], u16[2], u16[3],
        u16[4], u16[5], u16[6], u16[7]);
    return s;
}

char* XmmVal::ToString_x32(char* s, size_t len)
{
    sprintf_s(s, len, "%08X %08X | %08X %08X",
        u32[0], u32[1], u32[2], u32[3]);
    return s;
}

char* XmmVal::ToString_x64(char* s, size_t len)
{
    sprintf_s(s, len, "%25llX | %25llX", u64[0], u64[1]);
    return s;
}

char* XmmVal::ToString_r32(char* s, size_t len)
{
    sprintf_s(s, len, "%12.6f %12.6f | %12.6f %12.6f",
        r32[0], r32[1], r32[2], r32[3]);
    return s;
}

char* XmmVal::ToString_r64(char* s, size_t len)
{
    sprintf_s(s, len, "%24.12lf  | %24.12lf",
        r64[0], r64[1]);
    return s;
}
