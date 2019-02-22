#pragma once

#include "MiscDefs.h"

union MmxVal
{
    Int8  i8[8];
    Int16 i16[4];
    Int32 i32[2];
    Int64 i64;
    Uint8  u8[8];
    Uint16 u16[4];
    Uint32 u32[2];
    Uint64 u64;

    char* ToString_i8(char* s, size_t len);
    char* ToString_i16(char* s, size_t len);
    char* ToString_i32(char* s, size_t len);
    char* ToString_i64(char* s, size_t len);

    char* ToString_u8(char* s, size_t len);
    char* ToString_u16(char* s, size_t len);
    char* ToString_u32(char* s, size_t len);
    char* ToString_u64(char* s, size_t len);

    char* ToString_x8(char* s, size_t len);
    char* ToString_x16(char* s, size_t len);
    char* ToString_x32(char* s, size_t len);
    char* ToString_x64(char* s, size_t len);
};
