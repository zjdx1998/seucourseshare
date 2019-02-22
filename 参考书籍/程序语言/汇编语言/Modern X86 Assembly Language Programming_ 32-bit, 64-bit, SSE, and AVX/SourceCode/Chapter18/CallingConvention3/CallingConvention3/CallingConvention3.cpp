#include "stdafx.h"
#define _USE_MATH_DEFINES
#include <math.h>

extern "C" void Cc3_(const double* r, const double* h, int n, double* sa_cone, double* vol_cone);

int _tmain(int argc, _TCHAR* argv[])
{
    const int n = 6;
    double r[n] = { 1, 1, 2, 2, 3, 3 };
    double h[n] = { 1, 2, 3, 4, 5, 10 };
    double sa_cone1[n], sa_cone2[n];
    double vol_cone1[n], vol_cone2[n];

    // Calculate surface area and volume of right-circular cones
    for (int i = 0; i < n; i++)
    {
        sa_cone1[i] = M_PI * r[i] * (r[i] + sqrt(r[i] * r[i] + h[i] * h[i]));
        vol_cone1[i] = M_PI * r[i] * r[i] * h[i] / 3.0;
    }

    Cc3_(r, h, n, sa_cone2, vol_cone2);

    printf("\nResults for CallingConvention3\n");
    for (int i = 0; i < n; i++)
    {
        printf("  r/h: %14.2lf %14.2lf\n", r[i], h[i]);
        printf("  sa:  %14.6lf %14.6lf\n", sa_cone1[i], sa_cone2[i]);
        printf("  vol: %14.6lf %14.6lf\n", vol_cone1[i], vol_cone2[i]);
        printf("\n");
    }

    return 0;
}

