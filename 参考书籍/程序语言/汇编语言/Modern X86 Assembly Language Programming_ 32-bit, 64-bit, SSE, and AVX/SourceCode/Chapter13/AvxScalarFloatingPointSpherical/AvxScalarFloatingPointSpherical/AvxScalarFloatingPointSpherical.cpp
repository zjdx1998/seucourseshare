#include "stdafx.h"
#include <float.h>
#define _USE_MATH_DEFINES
#include <math.h>

extern "C" bool RectToSpherical_(const double r_coord[3], double s_coord[3]);
extern "C" bool SphericalToRect_(const double s_coord[3], double r_coord[3]);

extern "C" double DegToRad = M_PI / 180.0;
extern "C" double RadToDeg = 180.0 / M_PI;

void PrintCoord(const char* s, const double c[3])
{
      printf("%s %14.8lf %14.8lf %14.8lf\n", s, c[0], c[1], c[2]);
}

int _tmain(int argc, _TCHAR* argv[])
{
    const int n = 7;

    const double r_coords[n * 3] = 
    {
//      x coord     y coord         z coord
        2.0,        3.0,            6.0,
        -2.0,       -2.0,           2.0 * M_SQRT2,
        0.0,        M_SQRT2 / 2.0,  -M_SQRT2 / 2.0,
        M_SQRT2,    1.0,            -1.0,
        0.0,        0.0,            M_SQRT2,
        -1.0,       0.0,            0.0,
        0.0,        0.0,            0.0,
    };

    printf("Results for AvxScalarFloatingPointSpherical\n\n");

    for (int i = 0; i < n; i++)
    {
        double r_coord1[3], s_coord1[3], r_coord2[3];

        r_coord1[0] = r_coords[i * 3];
        r_coord1[1] = r_coords[i * 3 + 1];
        r_coord1[2] = r_coords[i * 3 + 2];

        RectToSpherical_(r_coord1, s_coord1);
        SphericalToRect_(s_coord1, r_coord2);

        PrintCoord("r_coord1 (x,y,z): ", r_coord1);
        PrintCoord("s_coord1 (r,t,p): ", s_coord1);
        PrintCoord("r_coord2 (x,y,z): ", r_coord2);
        printf("\n");
    }

    return 0;
}
