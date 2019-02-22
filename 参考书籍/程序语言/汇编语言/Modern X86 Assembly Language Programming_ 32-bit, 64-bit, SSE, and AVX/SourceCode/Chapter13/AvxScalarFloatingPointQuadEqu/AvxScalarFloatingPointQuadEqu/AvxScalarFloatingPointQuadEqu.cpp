#include "stdafx.h"
#include <math.h>

extern "C" void AvxSfpQuadEqu_(const double coef[3], double roots[2], double epsilon, int* dis);

void AvxSfpQuadEquCpp(const double coef[3], double roots[2], double epsilon, int* dis)
{
    double a = coef[0];
    double b = coef[1];
    double c = coef[2];
    double delta = b * b - 4.0 * a * c;
    double temp = 2.0 * a;

    if (fabs(a) < epsilon)
    {
        *dis = 9999;
        return;
    }

    if (fabs(delta) < epsilon)
    {
        roots[0] = -b / temp;
        roots[1] = -b / temp;
        *dis = 0;
    }
    else if (delta > 0)
    {
        roots[0] = (-b + sqrt(delta)) / temp;
        roots[1] = (-b - sqrt(delta)) / temp;
        *dis = 1;
    }
    else
    {
        // roots[0] contains real part, roots[1] contains imaginary part
        // complete answer is (r0, +r1), (r0, -r1)
        roots[0] = -b / temp;
        roots[1] = sqrt(-delta) / temp;
        *dis = -1;
    }
}

int _tmain(int argc, _TCHAR* argv[])
{
    const int n = 4;
    const double coef[n * 3] =
    {
        2.0, 8.0, -15.0,        // real roots (b * b > 4 * a * c)
        1.0, 6.0, 9.0,          // identical roots (b * b = 4 * a * c)
        3.0, 2.0, 4.0,          // complex roots (b * b < 4 * a * c)
        1.0e-13, 7.0, -5.0,     // invalid value for a
    };

    const double epsilon = 1.0e-12;

    printf("Results for AvxScalarFloatingPointQuadEqu\n");

    for (int i = 0; i < n * 3; i += 3)
    {
        double roots1[2], roots2[2];
        const double* coef2 = &coef[i];
        int dis1, dis2;

        AvxSfpQuadEquCpp(coef2, roots1, epsilon, &dis1);
        AvxSfpQuadEqu_(coef2, roots2, epsilon, &dis2);

        printf("\na: %lf, b: %lf c: %lf\n", coef2[0], coef2[1], coef2[2]);

        if (dis1 != dis2)
        {
            printf("Discriminant compare error\b");
            printf("  dis1/dis2: %d/%d\n", dis1, dis2);
        }

        switch (dis1)
        {
            case 1:
                printf("Distinct real roots\n");
                printf("  C++ roots: %lf %lf\n", roots1[0], roots1[1]);
                printf("  AVX roots: %lf %lf\n", roots2[0], roots2[1]);
                break;

            case 0:
                printf("Identical roots\n");
                printf("  C++ root: %lf\n", roots1[0]);
                printf("  AVX root: %lf\n", roots2[0]);
                break;

            case -1:
                printf("Complex roots\n");
                printf("  C++ roots: (%lf %lf) ", roots1[0], roots1[1]);
                printf("(%lf %lf)\n", roots1[0], -roots1[1]);
                printf("  AVX roots: (%lf %lf) ", roots2[0], roots2[1]);
                printf("(%lf %lf)\n", roots2[0], -roots2[1]);
                break;

            case 9999:
                printf("Coefficient 'a' is invalid\n");
                break;

            default:
                printf("Invalid discriminant value: %d\n", dis1);
                return 1;
        }
    }

    return 0;
}
