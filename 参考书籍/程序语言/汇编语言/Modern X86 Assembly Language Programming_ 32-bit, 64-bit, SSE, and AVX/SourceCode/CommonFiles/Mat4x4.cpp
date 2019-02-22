#include "stdafx.h"
#include "Mat4x4.h"

void Mat4x4Add(Mat4x4 des, Mat4x4 src1, Mat4x4 src2)
{
    des[0][0] = src1[0][0] + src2[0][0];
    des[0][1] = src1[0][1] + src2[0][1];
    des[0][2] = src1[0][2] + src2[0][2];
    des[0][3] = src1[0][3] + src2[0][3];

    des[1][0] = src1[1][0] + src2[1][0];
    des[1][1] = src1[1][1] + src2[1][1];
    des[1][2] = src1[1][2] + src2[1][2];
    des[1][3] = src1[1][3] + src2[1][3];

    des[2][0] = src1[2][0] + src2[2][0];
    des[2][1] = src1[2][1] + src2[2][1];
    des[2][2] = src1[2][2] + src2[2][2];
    des[2][3] = src1[2][3] + src2[2][3];

    des[3][0] = src1[3][0] + src2[3][0];
    des[3][1] = src1[3][1] + src2[3][1];
    des[3][2] = src1[3][2] + src2[3][2];
    des[3][3] = src1[3][3] + src2[3][3];
}

void Mat4x4Copy(Mat4x4 des, Mat4x4 src)
{
    des[0][0] = src[0][0];
    des[0][1] = src[0][1];
    des[0][2] = src[0][2];
    des[0][3] = src[0][3];

    des[1][0] = src[1][0];
    des[1][1] = src[1][1];
    des[1][2] = src[1][2];
    des[1][3] = src[1][3];

    des[2][0] = src[2][0];
    des[2][1] = src[2][1];
    des[2][2] = src[2][2];
    des[2][3] = src[2][3];

    des[3][0] = src[3][0];
    des[3][1] = src[3][1];
    des[3][2] = src[3][2];
    des[3][3] = src[3][3];
}

void Mat4x4Mul(Mat4x4 des, Mat4x4 src1, Mat4x4 src2)
{
    __declspec(align(32)) Mat4x4 src1_temp, src2_temp;

    if (des == src1)
    {
        Mat4x4Copy(src1_temp, src1);
        src1 = src1_temp;
    }

    if (des == src2)
    {
        Mat4x4Copy(src2_temp, src2);
        src2 = src2_temp;
    }

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            float sum = 0.0f;

            sum += src1[i][0] * src2[0][j];
            sum += src1[i][1] * src2[1][j];
            sum += src1[i][2] * src2[2][j];
            sum += src1[i][3] * src2[3][j];
            des[i][j] = sum;
        }
    }
}

void Mat4x4MulScalar(Mat4x4 des, Mat4x4 src, float s)
{
    des[0][0] = src[0][0] * s;
    des[0][1] = src[0][1] * s;
    des[0][2] = src[0][2] * s;
    des[0][3] = src[0][3] * s;

    des[1][0] = src[1][0] * s;
    des[1][1] = src[1][1] * s;
    des[1][2] = src[1][2] * s;
    des[1][3] = src[1][3] * s;

    des[2][0] = src[2][0] * s;
    des[2][1] = src[2][1] * s;
    des[2][2] = src[2][2] * s;
    des[2][3] = src[2][3] * s;

    des[3][0] = src[3][0] * s;
    des[3][1] = src[3][1] * s;
    des[3][2] = src[3][2] * s;
    des[3][3] = src[3][3] * s;
}

void Mat4x4MulVec(Vec4x1 des, Mat4x4 src1, Vec4x1 src2)
{
        Vec4x1 des_temp;

        for (int i = 0; i < 4; i++)
        {
            float sum = 0.0f;
            sum += src1[i][0] * src2[0];
            sum += src1[i][1] * src2[1];
            sum += src1[i][2] * src2[2];
            sum += src1[i][3] * src2[3];
            des_temp[i] = sum;
        }

        des[0] = des_temp[0];
        des[1] = des_temp[1];
        des[2] = des_temp[2];
        des[3] = des_temp[3];
}

void Mat4x4Printf(Mat4x4 m, const char* s)
{
    const char* f = "%14.6f %14.6f %14.6f %14.6f\n";
 
    printf("%s", s);
    printf(f, m[0][0], m[0][1], m[0][2], m[0][3]);
    printf(f, m[1][0], m[1][1], m[1][2], m[1][3]);
    printf(f, m[2][0], m[2][1], m[2][2], m[2][3]);
    printf(f, m[3][0], m[3][1], m[3][2], m[3][3]);
}

void Mat4x4SetI(Mat4x4 m)
{
    m[0][0] = 1;
    m[0][1] = 0;
    m[0][2] = 0;
    m[0][3] = 0;

    m[1][0] = 0;
    m[1][1] = 1;
    m[1][2] = 0;
    m[1][3] = 0;

    m[2][0] = 0;
    m[2][1] = 0;
    m[2][2] = 1;
    m[2][3] = 0;

    m[3][0] = 0;
    m[3][1] = 0;
    m[3][2] = 0;
    m[3][3] = 1;
}

float Mat4x4Trace(Mat4x4 x)
{
    return x[0][0] + x[1][1] + x[2][2] + x[3][3];
}

void Mat4x4Transpose(Mat4x4 des, Mat4x4 src)
{
    des[0][0] = src[0][0];
    des[0][1] = src[1][0];
    des[0][2] = src[2][0];
    des[0][3] = src[3][0];

    des[1][0] = src[0][1];
    des[1][1] = src[1][1];
    des[1][2] = src[2][1];
    des[1][3] = src[3][1];

    des[2][0] = src[0][2];
    des[2][1] = src[1][2];
    des[2][2] = src[2][2];
    des[2][3] = src[3][2];

    des[3][0] = src[0][3];
    des[3][1] = src[1][3];
    des[3][2] = src[2][3];
    des[3][3] = src[3][3];
}

void Vec4x1Printf(Vec4x1 v, const char* s)
{
    const char* f = "%14.6f %14.6f %14.6f %14.6f\n";

    printf("%s", s);
    printf(f, v[0], v[1], v[2], v[3]);
}
