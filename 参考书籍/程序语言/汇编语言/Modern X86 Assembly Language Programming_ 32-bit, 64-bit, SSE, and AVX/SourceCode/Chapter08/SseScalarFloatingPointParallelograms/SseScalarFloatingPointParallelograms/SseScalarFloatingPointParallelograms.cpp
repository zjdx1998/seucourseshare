#include "stdafx.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <stddef.h>

// Uncomment line below to enable display of PDATA information
//#define DISPLAY_PDATA_INFO

// This structure must agree with the structure that's defined
// in file SseScalarFloatingPointParallelograms_.asm.
typedef struct
{
    double A;               // Length of left and right
    double B;               // Length of top and bottom
    double Alpha;           // Angle alpha in degrees
    double Beta;            // Angle beta in degrees
    double H;               // Height of parallelogram
    double Area;            // Parallelogram area
    double P;               // Length of diagonal P
    double Q;               // Length of diagonal Q
    bool BadValue;          // Set to true if A, B, or Alpha is invalid
    char Pad[7];            // Reserved for future use
} PDATA;

extern "C" bool SseSfpParallelograms_(PDATA* pdata, int n);
extern "C" double DegToRad = M_PI / 180.0;
extern "C" int SizeofPdataX86_;
const bool PrintPdataInfo = true;

void SetPdata(PDATA* pdata, double a, double b, double alpha)
{
    pdata->A = a;
    pdata->B = b;
    pdata->Alpha = alpha;
}

int _tmain(int argc, _TCHAR* argv[])
{
#ifdef DISPLAY_PDATA_INFO
    size_t spd1 = sizeof(PDATA);
    size_t spd2 =  SizeofPdataX86_;

    if (spd1 != spd2)
        printf("PDATA size discrepancy [%d, %d]", spd1, spd2);
    else
    {    
        printf("sizeof(PDATA):      %d\n", spd1);
        printf("Offset of A:        %d\n", offsetof(PDATA, A));
        printf("Offset of B:        %d\n", offsetof(PDATA, B));
        printf("Offset of Alpha:    %d\n", offsetof(PDATA, Alpha));
        printf("Offset of Beta:     %d\n", offsetof(PDATA, Beta));
        printf("Offset of H         %d\n", offsetof(PDATA, H));
        printf("Offset of Area:     %d\n", offsetof(PDATA, Area));
        printf("Offset of P:        %d\n", offsetof(PDATA, P));
        printf("Offset of Q:        %d\n", offsetof(PDATA, Q));
        printf("Offset of BadValue  %d\n", offsetof(PDATA, BadValue));
        printf("Offset of Pad       %d\n", offsetof(PDATA, Pad));
    }
#endif

    const int n = 10;
    PDATA pdata[n];

    // Create some test parallelograms
    SetPdata(&pdata[0], -1.0, 1.0, 60.0);
    SetPdata(&pdata[1], 1.0, -1.0, 60.0);
    SetPdata(&pdata[2], 1.0, 1.0, 181.0);
    SetPdata(&pdata[3], 1.0, 1.0, 90.0);
    SetPdata(&pdata[4], 3.0, 4.0, 90.0);
    SetPdata(&pdata[5], 2.0, 3.0, 30.0);
    SetPdata(&pdata[6], 3.0, 2.0, 60.0);
    SetPdata(&pdata[7], 4.0, 2.5, 120.0);
    SetPdata(&pdata[8], 5.0, 7.125, 135.0);
    SetPdata(&pdata[9], 8.0, 8.0, 165.0);

    SseSfpParallelograms_(pdata, n);

    for (int i = 0; i < n; i++)
    {
        PDATA* p = &pdata[i];
        printf("\npdata[%d] - BadValue = %d\n", i, p->BadValue);
        printf("  A:      %12.6lf  B:    %12.6lf\n", p->A, p->B);
        printf("  Alpha:  %12.6lf  Beta: %12.6lf\n", p->Alpha, p->Beta);
        printf("  H:      %12.6lf  Area: %12.6lf\n", p->H, p->Area);
        printf("  P:      %12.6lf  Q:    %12.6lf\n", p->P, p->Q);
    }

    return 0;
}
