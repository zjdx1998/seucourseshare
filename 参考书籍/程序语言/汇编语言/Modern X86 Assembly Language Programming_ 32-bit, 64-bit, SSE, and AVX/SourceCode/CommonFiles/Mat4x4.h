#pragma once

typedef float Mat4x4[4][4];
typedef float Vec4x1[4];

extern void Mat4x4Add(Mat4x4 des, Mat4x4 src1, Mat4x4 src2);
extern void Mat4x4Copy(Mat4x4 des, Mat4x4 src);
extern void Mat4x4Mul(Mat4x4 des, Mat4x4 src1, Mat4x4 src2);
extern void Mat4x4MulScalar(Mat4x4 des, Mat4x4 src, float s);
extern void Mat4x4MulVec(Vec4x1 des, Mat4x4 src1, Vec4x1 src2);
extern void Mat4x4Printf(Mat4x4 m, const char* s);
extern void Mat4x4SetI(Mat4x4 m);
extern float Mat4x4Trace(Mat4x4 m);
extern void Mat4x4Transpose(Mat4x4 des, Mat4x4 src);
extern void Vec4x1Printf(Vec4x1 v, const char* s);

inline void Mat4x4SetRow(Mat4x4 m, int row, float a0, float a1, float a2, float a3)
{
    m[row][0] = a0;
    m[row][1] = a1;
    m[row][2] = a2;
    m[row][3] = a3;
}

inline void Vec4x1Set(Vec4x1 v, float a0, float a1, float a2, float a3)
{
    v[0] = a0;
    v[1] = a1;
    v[2] = a2;
    v[3] = a3;
}
