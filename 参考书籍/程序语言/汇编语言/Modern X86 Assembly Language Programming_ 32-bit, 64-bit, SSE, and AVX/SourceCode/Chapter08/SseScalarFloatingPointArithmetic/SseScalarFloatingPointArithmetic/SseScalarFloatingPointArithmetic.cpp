#include "stdafx.h"
#define _USE_MATH_DEFINES
#include <math.h>

extern "C" void SseSfpArithmeticFloat_(float a, float b, float c[8]);
extern "C" void SseSfpArithmeticDouble_(double a, double b, double c[8]);

void SseSpfArithmeticFloat(void)
{
    float a = 2.5f;
    float b = -7.625f;
    float c[8];

    SseSfpArithmeticFloat_(a, b, c);
    printf("\nResults for SseSfpArithmeticFloat_()\n");
    printf("  a:             %.6f\n", a);
    printf("  b:             %.6f\n", b);
    printf("  add:           %.6f\n", c[0]);
    printf("  sub:           %.6f\n", c[1]);
    printf("  mul:           %.6f\n", c[2]);
    printf("  div:           %.6f\n", c[3]);
    printf("  min:           %.6f\n", c[4]);
    printf("  max:           %.6f\n", c[5]);
    printf("  fabs(b):       %.6f\n", c[6]);
    printf("  sqrt(fabs(b)): %.6f\n", c[7]);
}

void SseSpfArithmeticDouble(void)
{
    double a = M_PI;
    double b = M_E;
    double c[8];

    SseSfpArithmeticDouble_(a, b, c);
    printf("\nResults for SseSfpArithmeticDouble_()\n");
    printf("  a:             %.14f\n", a);
    printf("  b:             %.14f\n", b);
    printf("  add:           %.14f\n", c[0]);
    printf("  sub:           %.14f\n", c[1]);
    printf("  mul:           %.14f\n", c[2]);
    printf("  div:           %.14f\n", c[3]);
    printf("  min:           %.14f\n", c[4]);
    printf("  max:           %.14f\n", c[5]);
    printf("  fabs(b):       %.14f\n", c[6]);
    printf("  sqrt(fabs(b)): %.14f\n", c[7]);
}

int _tmain(int argc, _TCHAR* argv[])
{
    SseSpfArithmeticFloat();
    SseSpfArithmeticDouble();
}
