#include "stdafx.h"
#include "SsePackedFloatingPointMatrix4x4.h"

// The functions Mat4x4Mul and Mat4x4MulVec are defined in
// the file CommonFiles\Mat4x4.cpp

void SsePfpMatrix4x4MultiplyCpp(Mat4x4 m_des, Mat4x4 m_src1, Mat4x4 m_src2)
{
    Mat4x4Mul(m_des, m_src1, m_src2);
}

void SsePfpMatrix4x4TransformVectorsCpp(Vec4x1* v_des, Mat4x4 m_src, Vec4x1* v_src, int num_vec)
{
    for (int i= 0; i < num_vec; i++)
        Mat4x4MulVec(v_des[i], m_src, v_src[i]);
}

void SsePfpMatrix4x4Multiply(void)
{
    __declspec(align(16)) Mat4x4 m_src1;
    __declspec(align(16)) Mat4x4 m_src2;
    __declspec(align(16)) Mat4x4 m_des1;
    __declspec(align(16)) Mat4x4 m_des2;

    Mat4x4SetRow(m_src1, 0, 10.5, 11, 12, -13.625);
    Mat4x4SetRow(m_src1, 1, 14, 15, 16, 17.375);
    Mat4x4SetRow(m_src1, 2, 18.25, 19, 20.125, 21);
    Mat4x4SetRow(m_src1, 3, 22, 23.875, 24, 25);

    Mat4x4SetRow(m_src2, 0, 7, 1, 4, 8);
    Mat4x4SetRow(m_src2, 1, 14, -5, 2, 9);
    Mat4x4SetRow(m_src2, 2, 10, 9, 3, 6);
    Mat4x4SetRow(m_src2, 3, 2, 11, -14, 13);

    SsePfpMatrix4x4MultiplyCpp(m_des1, m_src1, m_src2);
    SsePfpMatrix4x4Multiply_(m_des2, m_src1, m_src2);

    printf("\nResults for SsePfpMatrix4x4Multiply()\n");
    Mat4x4Printf(m_src1, "\nMatrix m_src1\n");
    Mat4x4Printf(m_src2, "\nMatrix m_src2\n");
    Mat4x4Printf(m_des1, "\nMatrix m_des1\n");
    Mat4x4Printf(m_des2, "\nMatrix m_des2\n");
}

void SsePfpMatrix4x4TransformVectors(void)
{
    const int n = 8;
    __declspec(align(16)) Mat4x4 m_src;
    __declspec(align(16)) Vec4x1 v_src[n];
    __declspec(align(16)) Vec4x1 v_des1[n];
    __declspec(align(16)) Vec4x1 v_des2[n];

    Vec4x1Set(v_src[0], 10, 10, 10, 1);
    Vec4x1Set(v_src[1], 10, 11, 10, 1);
    Vec4x1Set(v_src[2], 11, 10, 10, 1);
    Vec4x1Set(v_src[3], 11, 11, 10, 1);
    Vec4x1Set(v_src[4], 10, 10, 12, 1);
    Vec4x1Set(v_src[5], 10, 11, 12, 1);
    Vec4x1Set(v_src[6], 11, 10, 12, 1);
    Vec4x1Set(v_src[7], 11, 11, 12, 1);

    // m_src = scale(2, 3, 4)
    Mat4x4SetRow(m_src, 0, 2, 0, 0, 0);
    Mat4x4SetRow(m_src, 1, 0, 3, 0, 0);
    Mat4x4SetRow(m_src, 2, 0, 0, 7, 0);
    Mat4x4SetRow(m_src, 3, 0, 0, 0, 1);

    SsePfpMatrix4x4TransformVectorsCpp(v_des1, m_src, v_src, n);
    SsePfpMatrix4x4TransformVectors_(v_des2, m_src, v_src, n);

    printf("\nResults for SsePfpMatrix4x4TransformVectors()\n");
    Mat4x4Printf(m_src, "Matrix m_src\n");
    printf("\n");

    for (int i = 0; i < n; i++)
    {
        const char* fmt = "%4s %4d: %12.6f %12.6f %12.6f %12.6f\n";
        printf(fmt, "v_src  ", i, v_src[i][0],  v_src[i][1],  v_src[i][2],  v_src[i][3]);
        printf(fmt, "v_des1 ", i, v_des1[i][0], v_des1[i][1], v_des1[i][2], v_des1[i][3]);
        printf(fmt, "v_des2 ", i, v_des2[i][0], v_des2[i][1], v_des2[i][2], v_des2[i][3]);
        printf("\n");
    }
}

int _tmain(int argc, _TCHAR* argv[])
{
    SsePfpMatrix4x4Multiply();
    SsePfpMatrix4x4TransformVectors();

    SsePfpMatrix4x4MultiplyTimed();
    SsePfpMatrix4x4TransformVectorsTimed();
    return 0;
}
