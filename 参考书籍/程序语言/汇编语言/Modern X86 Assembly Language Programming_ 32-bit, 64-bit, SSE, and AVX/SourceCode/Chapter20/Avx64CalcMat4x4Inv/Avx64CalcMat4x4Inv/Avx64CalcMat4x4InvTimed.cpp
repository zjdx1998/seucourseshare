#include "stdafx.h"
#include "Avx64CalcMat4x4Inv.h"
#include "ThreadTimer.h"

void Avx64CalcMat4x4InvTimed(void)
{
    ThreadTimer::SetThreadAffinityMask();
    const int num_it = 100;
    const int num_alg = 2;
    const double et_scale = 1.0e6;
    double et[num_it][num_alg];
    ThreadTimer tt;

    const float epsilon = 1.0e-9f;
    const int num_inv = 10000;
    __declspec(align(32)) Mat4x4 m;
    __declspec(align(32)) Mat4x4 m_inv1;
    __declspec(align(32)) Mat4x4 m_inv2;
    bool rc, is_singular;

    Mat4x4SetRow(m, 0, 2, 7, 3, 4);
    Mat4x4SetRow(m, 1, 5, 9, 6, 4.75);
    Mat4x4SetRow(m, 2, 6.5, 3, 4, 10);
    Mat4x4SetRow(m, 3, 7, 5.25, 8.125, 6);

    for (int i = 0; i < num_it; i++)
    {
        tt.Start();
        for (int j = 0; j < num_inv; j++)
            rc = Mat4x4InvCpp(m_inv1, m, epsilon, &is_singular);
        tt.Stop();
        et[i][0] = tt.GetElapsedTime() * et_scale;
    }

    for (int i = 0; i < num_it; i++)
    {
        tt.Start();
        for (int j = 0; j < num_inv; j++)
            rc = Mat4x4Inv_(m_inv2, m, epsilon, &is_singular);
        tt.Stop();
        et[i][1] = tt.GetElapsedTime() * et_scale;
    }

    const char* fn = "__Avx64CalcMat4x4InvTimed.csv";
    ThreadTimer::SaveElapsedTimeMatrix(fn, (double*)et, num_it, num_alg);
    printf("\nBenchmark times saved to file %s\n", fn);
}
