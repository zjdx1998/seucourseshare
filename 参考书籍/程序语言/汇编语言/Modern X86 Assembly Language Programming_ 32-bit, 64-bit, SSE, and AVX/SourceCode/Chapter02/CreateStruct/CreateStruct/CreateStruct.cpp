#include "stdafx.h"
#include "TestStruct.h"

extern "C" TestStruct* CreateTestStruct_(__int8 val8, __int16 val16, __int32 val32, __int64 val64);
extern "C" void ReleaseTestStruct_(TestStruct* p);

void PrintTestStruct(const char* msg, const TestStruct* ts)
{
    printf("%s\n", msg);
    printf("  ts->Val8:   %d\n", ts->Val8);
    printf("  ts->Val16:  %d\n", ts->Val16);
    printf("  ts->Val32:  %d\n", ts->Val32);
    printf("  ts->Val64:  %lld\n", ts->Val64);
}

int _tmain(int argc, _TCHAR* argv[])
{
    TestStruct* ts = CreateTestStruct_(40, -401, 400002, -4000000003LL);

    PrintTestStruct("Contents of TestStruct 'ts'", ts);

    ReleaseTestStruct_(ts);
    return 0;
}
