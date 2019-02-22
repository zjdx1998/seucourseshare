#include "stdafx.h"
#include <math.h>
#include <stdlib.h>

extern "C" __declspec(align(32)) double CcEpsilon = 1.0e-12;
extern "C" bool AvxPfpCorrCoef_(const double* x, const double* y, int n, double sums[5], double* rho);

bool AvxPfpCorrCoefCpp(const double* x, const double* y, int n, double sums[5], double* rho)
{
    double sum_x = 0, sum_y = 0;
    double sum_xx = 0, sum_yy = 0, sum_xy = 0;

    // Make sure x and y are properly aligned to a 32-byte boundary
    if (((uintptr_t)x & 0x1f) != 0)
        return false;
    if (((uintptr_t)y & 0x1f) != 0)
        return false;

    // Make sure n is valid
    if ((n < 4) || ((n & 3) != 0))
        return false;

    // Calculate and save sum variables
    for (int i = 0; i < n; i++)
    {
        sum_x += x[i];
        sum_y += y[i];
        sum_xx += x[i] * x[i];
        sum_yy += y[i] * y[i];
        sum_xy += x[i] * y[i];
    }

    sums[0] = sum_x;
    sums[1] = sum_y;
    sums[2] = sum_xx;
    sums[3] = sum_yy;
    sums[4] = sum_xy;

    // Calculate rho
    double rho_num = n * sum_xy - sum_x * sum_y;
    double rho_den = sqrt(n * sum_xx - sum_x * sum_x) * sqrt(n * sum_yy - sum_y * sum_y);

    if (rho_den >= CcEpsilon)
    {
        *rho = rho_num / rho_den;
        return true;
    }
    else
    {
        *rho = 0;
        return false;
    }
}

int _tmain(int argc, _TCHAR* argv[])
{
    const int n = 100;
    __declspec(align(32)) double x[n];
    __declspec(align(32)) double y[n];
    double sums1[5], sums2[5];
    double rho1, rho2;

    srand(17);
    for (int i = 0; i < n; i++)
    {
        x[i] = rand();
        y[i] = x[i] + ((rand() % 6000) - 3000);
    }

    bool rc1 = AvxPfpCorrCoefCpp(x, y, n, sums1, &rho1);
    bool rc2 = AvxPfpCorrCoef_(x, y, n, sums2, &rho2);

    printf("Results for AvxPackedFloatingPointCorrCoef\n\n");

    if (!rc1 || !rc2)
    {
        printf("Invalid return code (rc1: %d, rc2: %d)\n", rc1, rc2);
        return 1;
    }

    printf("rho1: %.8lf  rho2: %.8lf\n", rho1, rho2);
    printf("\n");
    printf("sum_x:  %12.0lf %12.0lf\n", sums1[0], sums2[0]);
    printf("sum_y:  %12.0lf %12.0lf\n", sums1[1], sums2[1]);
    printf("sum_xx: %12.0lf %12.0lf\n", sums1[2], sums2[2]);
    printf("sum_yy: %12.0lf %12.0lf\n", sums1[3], sums2[3]);
    printf("sum_xy: %12.0lf %12.0lf\n", sums1[4], sums2[4]);
    return 0;
}
