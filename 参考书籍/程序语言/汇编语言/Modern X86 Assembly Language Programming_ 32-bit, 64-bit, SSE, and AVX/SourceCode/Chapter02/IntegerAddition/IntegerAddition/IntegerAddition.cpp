#include "stdafx.h"

extern "C" char GlChar = 10;
extern "C" short GlShort = 20;
extern "C" int GlInt = 30;
extern "C" long long GlLongLong = 0x000000000FFFFFFFE;

extern "C" void IntegerAddition_(char a, short b, int c, long long d);

int _tmain(int argc, _TCHAR* argv[])
{
    printf("Before GlChar:     %d\n", GlChar);
    printf("       GlShort:    %d\n", GlShort);
    printf("       GlInt:      %d\n", GlInt);
    printf("       GlLongLong: %lld\n", GlLongLong);
    printf("\n");

    IntegerAddition_(3, 5, -37, 11);

    printf("After  GlChar:     %d\n", GlChar);
    printf("       GlShort:    %d\n", GlShort);
    printf("       GlInt:      %d\n", GlInt);
    printf("       GlLongLong: %lld\n", GlLongLong);
    return 0;
}
