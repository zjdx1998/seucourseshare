#include "stdafx.h"
#include "Avx64CalcMat4x4Inv.h"

extern "C" void TestMat4x4(Mat4x4 m1, Mat4x4 m2, float* m1_trace, Mat4x4 m3);

void Avx64CalcMat4x4InvTest(void)
{
    float m1_trace;
    __declspec(align(32)) Mat4x4 m1;
    __declspec(align(32)) Mat4x4 m2;
    __declspec(align(32)) Mat4x4 m3;
    __declspec(align(32)) Mat4x4 m4;

    Mat4x4SetRow(m1, 0, 7, 2, 19, 3);
    Mat4x4SetRow(m1, 1, 8, 6, 5, 10);
    Mat4x4SetRow(m1, 2, 22, 3, 1, 12);
    Mat4x4SetRow(m1, 3, 13, 25, 9, 4);
    Mat4x4Printf(m1, "\nMatrix m1\n");

    Mat4x4SetRow(m2, 0, 6, 10, 3, 7);
    Mat4x4SetRow(m2, 1, 5, 2, 12, 11);
    Mat4x4SetRow(m2, 2, 13, 4, 3, 9);
    Mat4x4SetRow(m2, 3, 8, 5, 1, 2);
    Mat4x4Printf(m2, "\nMatrix m2\n");

    TestMat4x4(m1, m2, &m1_trace, m3);
    Mat4x4Printf(m3, "\nMatrix m3 = m1 * m2\n");

    Mat4x4Mul(m4, m1, m2);
    Mat4x4Printf(m4, "\nMatrix m4 = m1 * m2\n");

    printf("\nm1_trace: %f\n\n", m1_trace);
}