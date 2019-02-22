#include "stdafx.h"
#include <math.h>

extern "C" bool CalcMeanStdev_(const double* a, int n, double* mean, double* stdev);

bool CalcMeanStdevCpp(const double* a, int n, double* mean, double* stdev)
{
        if (n <= 1)
            return false;

        double sum = 0.0;
        for (int i = 0; i < n; i++)
            sum += a[i];
        *mean = sum / n;

        sum = 0.0;
        for (int i = 0; i < n; i++)
        {
                double temp = a[i] - *mean;
                sum += temp * temp;
        }

        *stdev = sqrt(sum / (n - 1));
        return true;
}

int _tmain(int argc, _TCHAR* argv[])
{
    double a[] = { 10, 2, 33, 15, 41, 24, 75, 37, 18, 97};
    const int n = sizeof(a) / sizeof(double);
    double mean1, stdev1;
    double mean2, stdev2;

    CalcMeanStdevCpp(a, n, &mean1, &stdev1);
    CalcMeanStdev_(a, n, &mean2, &stdev2);

    for (int i = 0; i < n; i++)
        printf("a[%d] = %g\n", i, a[i]);

    printf("\n");
    printf("mean1: %g stdev1: %g\n", mean1, stdev1);
    printf("mean2: %g stdev2: %g\n", mean2, stdev2);
}
