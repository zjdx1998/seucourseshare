#include "stdafx.h"
#include "SsePackedFloatingPointMatrix4x4.h"
#include "ThreadTimer.h"
#include <stdlib.h>
#include <stdio.h>

void SsePfpMatrix4x4MultiplyTimed(void)
{
    // Force current thread to execute on a single processor
    ThreadTimer::SetThreadAffinityMask();

    const int n = 2000;
    __declspec(align(16)) Mat4x4 m_src1[n];
    __declspec(align(16)) Mat4x4 m_src2[n];
    __declspec(align(16)) Mat4x4 m_des1[n];
    __declspec(align(16)) Mat4x4 m_des2[n];

    for (int i = 0; i < n; i++)
    {
        Mat4x4SetRow(m_src1[i], 0, 10, 11, 12, 13);
        Mat4x4SetRow(m_src1[i], 1, 14, 15, 16, 17);
        Mat4x4SetRow(m_src1[i], 2, 18, 19, 20, 21);
        Mat4x4SetRow(m_src1[i], 3, 22, 23, 24, 25);

        Mat4x4SetRow(m_src2[i], 0, 0, 1, 2, 3);
        Mat4x4SetRow(m_src2[i], 1, 4, 5, 6, 7);
        Mat4x4SetRow(m_src2[i], 2, 8, 9, 10, 11);
        Mat4x4SetRow(m_src2[i], 3, 12, 13, 14, 15);
    }

    const int num_it = 100;
    const int num_alg = 2;
    const double et_scale = 1.0e6;
    double et[num_it][num_alg];
    ThreadTimer tt;

    for (int i = 0; i < num_it; i++)
    {
        tt.Start();
        for (int j = 0; j < n; j++)
            SsePfpMatrix4x4MultiplyCpp(m_des1[j], m_src1[j], m_src2[j]);
        tt.Stop();
        et[i][0] = tt.GetElapsedTime() * et_scale;
    }

    for (int i = 0; i < num_it; i++)
    {
        tt.Start();
        for (int j = 0; j < n; j++)
            SsePfpMatrix4x4Multiply_(m_des2[j], m_src1[j], m_src2[j]);
        tt.Stop();
        et[i][1] = tt.GetElapsedTime() * et_scale;
    }

    char buff[256];
    sprintf_s(buff, sizeof(buff), "%p, %p", m_des1, m_des2);

    const char* fn = "__SsePfpMatrix4x4MultiplyTimed.csv";
    ThreadTimer::SaveElapsedTimeMatrix(fn, (double*)et, num_it, num_alg);
    printf("\nResults for SsePfpMatrix4x4MultiplyTimed()\n");
    printf("Benchmark times saved to %s\n", fn);
}

void SsePfpMatrix4x4TransformVectorsTimed(void)
{
    // Force current thread to execute on a single processor
    ThreadTimer::SetThreadAffinityMask();

    const bool display_results = false;
    const int n = 10000;
    __declspec(align(16)) Mat4x4 m_src;
    __declspec(align(16)) Vec4x1 v_src[n];
    __declspec(align(16)) Vec4x1 v_des1[n];
    __declspec(align(16)) Vec4x1 v_des2[n];

    //:: Scale 2,3,4 (x,y,z)
    Mat4x4SetRow(m_src, 0, 2, 0, 0, 0);
    Mat4x4SetRow(m_src, 1, 0, 3, 0, 0);
    Mat4x4SetRow(m_src, 2, 0, 0, 4, 0);
    Mat4x4SetRow(m_src, 3, 0, 0, 0, 1);

    srand(37);

    for (int i = 0; i < n; i++)
    {
        float f1 = (float)(rand() % 1000) / 999.0f;
        float f2 = (float)(rand() % 1000) / 999.0f;
        float f3 = (float)(rand() % 1000) / 999.0f;
        Vec4x1Set(v_src[i], f1, f2, f3, 1.0f);
    }

    const int num_it = 100;
    const int num_alg = 2;
    const double et_scale = 1.0e6;
    double et[num_it][num_alg];
    ThreadTimer tt;

    for (int i = 0; i < num_it; i++)
    {
        tt.Start();
        SsePfpMatrix4x4TransformVectorsCpp(v_des1, m_src, v_src, n);
        tt.Stop();
        et[i][0] = tt.GetElapsedTime() * et_scale;
    }

    for (int i = 0; i < num_it; i++)
    {
        tt.Start();
        SsePfpMatrix4x4TransformVectors_(v_des2, m_src, v_src, n);
        tt.Stop();
        et[i][1] = tt.GetElapsedTime() * et_scale;
    }

    if (display_results)
    {
        for (int i = 0; i < n; i++)
        {
            printf("src  %4d: %12.6f %12.6f %12.6f %12.6f\n", i, v_src[i][0], v_src[i][1], v_src[i][2], v_src[i][3]);
            printf("des0 %4d: %12.6f %12.6f %12.6f %12.6f\n", i, v_des1[i][0], v_des1[i][1], v_des1[i][2], v_des1[i][3]);
            printf("des1 %4d: %12.6f %12.6f %12.6f %12.6f\n", i, v_des2[i][0], v_des2[i][1], v_des2[i][2], v_des2[i][3]);
            printf("\n");
        }
    }

    char buff[256];
    sprintf_s(buff, sizeof(buff), "%p, %p", v_des1, v_des2);

    const char* fn = "__SsePfpMatrix4x4TransformVectorsTimed.csv";
    ThreadTimer::SaveElapsedTimeMatrix(fn, (double*)et, num_it, num_alg);
    printf("\nResults for SsePfpMatrix4x4TransformVectorsTimed()\n");
    printf("Benchmark times saved to %s\n", fn);
}
