#include "stdafx.h"

extern "C" bool CalcResult4_(int* y, const int* x, int n);

int _tmain(int argc, _TCHAR* argv[])
{
    const int n = 8;
    const int x[n] = {3, 2, 5, 7, 8, 13, 20, 25};
    int y[n];

    CalcResult4_(y, x, n);

#ifdef _WIN64
    const char* sp = "x64";
#else
    const char* sp = "Win32";
#endif

    printf("Results for solution platform %s\n\n", sp);
    printf("     x      y\n");
    printf("--------------\n");

    for (int i = 0; i < n; i++)
        printf("%6d %6d\n", x[i], y[i]); 
    return 0;
}
