#include "stdafx.h"
#include <stdlib.h>

extern "C" int CompareArrays_(const int* x, const int* y, int n);

int _tmain(int argc, _TCHAR* argv[])
{
    const int n = 21;
    int x[n], y[n];
    int result;

    // Initialize test arrays
    srand(11);
    for (int i = 0; i < n; i++)
        x[i] = y[i] = rand() % 1000;

    printf("\nResults for CompareArrays\n");

    // Test using invalid 'n'
    result = CompareArrays_(x, y, -n);
    printf("  Test #1 - expected: %3d  actual: %3d\n", -1, result);

    // Test using first element mismatch
    x[0] += 1;
    result = CompareArrays_(x, y, n);
    x[0] -= 1;
    printf("  Test #2 - expected: %3d  actual: %3d\n", 0, result);

    // Test using middle element mismatch
    y[n / 2] -= 2;
    result = CompareArrays_(x, y, n);
    y[n / 2] += 2;
    printf("  Test #3 - expected: %3d  actual: %3d\n", n / 2, result);

    // Test using last element mismatch
    x[n - 1] *= 3;
    result = CompareArrays_(x, y, n);
    x[n - 1] /= 3;
    printf("  Test #4 - expected: %3d  actual: %3d\n", n - 1, result);

    // Test with identical elements in each array
    result = CompareArrays_(x, y, n);
    printf("  Test #5 - expected: %3d  actual: %3d\n", n, result);
    return 0;
}
