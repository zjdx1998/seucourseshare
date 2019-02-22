#include "stdafx.h"
#include <stddef.h>
#include <math.h>

extern "C" double LsEpsilon = 1.0e-12;
extern "C" bool SsePfpLeastSquares_(const double* x, const double* y, int n, double* m, double* b);

bool SsePfpLeastSquaresCpp(const double* x, const double* y, int n, double* m, double* b)
{
    if (n < 2)
        return false;

    // Make sure x and y are properly aligned
    if ((((uintptr_t)x & 0xf) != 0) || (((uintptr_t)y & 0xf) != 0))
        return false;

    double sum_x = 0, sum_y = 0.0, sum_xx = 0, sum_xy = 0.0;

    for (int i = 0; i < n; i++)
    {
        sum_x += x[i];
        sum_xx += x[i] * x[i];
        sum_xy += x[i] * y[i];
        sum_y += y[i];
    }

    double denom = n * sum_xx - sum_x * sum_x;

    if (fabs(denom) >= LsEpsilon)
    {
        *m = (n * sum_xy - sum_x * sum_y) / denom;
        *b = (sum_xx * sum_y - sum_x * sum_xy) / denom;
        return true;
    }
    else
    {
        *m = *b = 0.0;
        return false;
    }
}

int _tmain(int argc, _TCHAR* argv[])
{
    const int n = 11;
    __declspec(align(16)) double x[n] = {10, 13, 17, 19, 23, 7, 35, 51, 89, 92, 99};
    __declspec(align(16)) double y[n] = {1.2, 1.1, 1.8, 2.2, 1.9, 0.5, 3.1, 5.5, 8.4, 9.7, 10.4};

    double m1, m2, b1, b2;
    bool rc1 = SsePfpLeastSquaresCpp(x, y, n, &m1, &b1);
    bool rc2 = SsePfpLeastSquares_(x, y, n, &m2, &b2);

    printf("\nResults from SsePackedFloatingPointLeastSquaresCpp\n");
    printf("  rc:        %12d\n", rc1);
    printf("  slope:     %12.8lf\n", m1);
    printf("  intercept: %12.8lf\n", b1);
    printf("\nResults from SsePackedFloatingPointLeastSquares_\n");
    printf("  rc:        %12d\n", rc2);
    printf("  slope:     %12.8lf\n", m2);
    printf("  intercept: %12.8lf\n", b2);
    return 0;
}
