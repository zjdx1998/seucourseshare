#include "stdafx.h"
#include "XmmVal.h"
#include "YmmVal.h"
#include <stdlib.h>

extern "C" void AvxGatherFloat_(YmmVal* des, YmmVal* indices, YmmVal* mask, const float* x);
extern "C" void AvxGatherI64_(YmmVal* des, XmmVal* indices, YmmVal* mask, const Int64* x);

void AvxGatherFloatPrint(const char* msg, YmmVal& des, YmmVal& indices, YmmVal& mask)
{
    printf("\n%s\n", msg);

    for (int i = 0; i < 8; i++)
    {
        printf("ElementID: %d  ", i);
        printf("des: %8.1f  ", des.r32[i]);
        printf("indices: %4d  ", indices.i32[i]);
        printf("mask: 0x%08X\n", mask.i32[i]);
    }
}

void AvxGatherI64Print(const char* msg, YmmVal& des, XmmVal& indices, YmmVal& mask)
{
    printf("\n%s\n", msg);

    for (int i = 0; i < 4; i++)
    {
        printf("ElementID: %d  ", i);
        printf("des: %8lld  ", des.i64[i]);
        printf("indices: %4d  ", indices.i32[i]);
        printf("mask: 0x%016llX\n", mask.i64[i]);
    }
}

void AvxGatherFloat(void)
{
    const int merge_no = 0;
    const int merge_yes = 0x80000000;
    const int n = 15;
    float x[n];
    __declspec(align(32)) YmmVal des;
    __declspec(align(32)) YmmVal indices;
    __declspec(align(32)) YmmVal mask;

    // Initialize the test array
    srand(22);
    for (int i = 0; i < n; i++)
        x[i] = (float)(rand() % 1000);

    // Load des with initial values
    for (int i = 0; i < 8; i++)
        des.r32[i] = -1.0f;

    // Initialize the indices
    indices.i32[0] = 2;
    indices.i32[1] = 1;
    indices.i32[2] = 6;
    indices.i32[3] = 5;
    indices.i32[4] = 4;
    indices.i32[5] = 13;
    indices.i32[6] = 11;
    indices.i32[7] = 9;

    // Initialize the mask value
    mask.i32[0] = merge_yes;
    mask.i32[1] = merge_yes;
    mask.i32[2] = merge_no;
    mask.i32[3] = merge_yes;
    mask.i32[4] = merge_yes;
    mask.i32[5] = merge_no;
    mask.i32[6] = merge_yes;
    mask.i32[7] = merge_yes;

    printf("\nResults for AvxGatherFloat()\n");
    printf("Test array\n");
    for (int i = 0; i < n; i++)
        printf("x[%02d]: %6.1f\n", i, x[i]);
    printf("\n");

    const char* s1 = "Values BEFORE call to AvxGatherFloat_()";
    const char* s2 = "Values AFTER call to AvxGatherFloat_()";

    AvxGatherFloatPrint(s1, des, indices, mask);
    AvxGatherFloat_(&des, &indices, &mask, x);
    AvxGatherFloatPrint(s2, des, indices, mask);
}

void AvxGatherI64(void)
{
    const Int64 merge_no = 0;
    const Int64 merge_yes = 0x8000000000000000LL;
    const int n = 15;
    Int64 x[n];
    __declspec(align(32)) YmmVal des;
    __declspec(align(16)) XmmVal indices;
    __declspec(align(32)) YmmVal mask;

    // Initialize the test array
    srand(36);
    for (int i = 0; i < n; i++)
        x[i] = (Int64)(rand() % 1000);

    // Load des with initial values
    for (int i = 0; i < 4; i++)
        des.i64[i] = -1;

    // Initialize the indices and mask elements
    indices.i32[0] = 2;
    indices.i32[1] = 7;
    indices.i32[2] = 9;
    indices.i32[3] = 12;

    mask.i64[0] = merge_yes;
    mask.i64[1] = merge_yes;
    mask.i64[2] = merge_no;
    mask.i64[3] = merge_yes;

    printf("\nResults for AvxGatherI64()\n");
    printf("Test array\n");
    for (int i = 0; i < n; i++)
        printf("x[%02d]: %8lld\n", i, x[i]);
    printf("\n");

    const char* s1 = "Values BEFORE call to AvxGatherI64_()";
    const char* s2 = "Values AFTER call to AvxGatherI64_()";

    AvxGatherI64Print(s1, des, indices, mask);
    AvxGatherI64_(&des, &indices, &mask, x);
    AvxGatherI64Print(s2, des, indices, mask);
}

int _tmain(int argc, _TCHAR* argv[])
{
    AvxGatherFloat();
    AvxGatherI64();
    return 0;
}
