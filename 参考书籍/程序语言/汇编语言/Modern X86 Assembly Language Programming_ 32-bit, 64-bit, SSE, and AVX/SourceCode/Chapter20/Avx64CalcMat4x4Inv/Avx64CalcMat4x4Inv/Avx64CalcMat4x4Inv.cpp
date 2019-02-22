#include "stdafx.h"
#include <math.h>
#include "Avx64CalcMat4x4Inv.h"

//#define MAT_INV_DEBUG     // Remove comment to enable extra printfs

bool Mat4x4InvCpp(Mat4x4 m_inv, Mat4x4 m, float epsilon, bool* is_singular)
{
    __declspec(align(32)) Mat4x4 m2;
    __declspec(align(32)) Mat4x4 m3;
    __declspec(align(32)) Mat4x4 m4;
    float t1, t2, t3, t4;
    float c1, c2, c3, c4;

    // Make sure matrices are properly aligned
    if (((uintptr_t)m_inv & 0x1f) != 0)
        return false;
    if (((uintptr_t)m & 0x1f) != 0)
        return false;

    // Calculate the required matrix trace values
    Mat4x4Mul(m2, m, m);
    Mat4x4Mul(m3, m2, m);
    Mat4x4Mul(m4, m3, m);
    t1 = Mat4x4Trace(m);
    t2 = Mat4x4Trace(m2);
    t3 = Mat4x4Trace(m3);
    t4 = Mat4x4Trace(m4);

#ifdef MAT_INV_DEBUG
    printf("t1: %16e\n", t1);
    printf("t2: %16e\n", t2);
    printf("t3: %16e\n", t3);
    printf("t4: %16e\n", t4);
#endif

    c1 = -t1;
    c2 = -1.0f / 2.0f * (c1 * t1 + t2);
    c3 = -1.0f / 3.0f * (c2 * t1 + c1 * t2 + t3);
    c4 = -1.0f / 4.0f * (c3 * t1 + c2 * t2 + c1 * t3 + t4);

#ifdef MAT_INV_DEBUG
    printf("c1: %16e\n", c1);
    printf("c2: %16e\n", c2);
    printf("c3: %16e\n", c3);
    printf("c4: %16e\n", c4);
#endif

    // Make sure matrix is not singular
    if ((*is_singular = (fabs(c4) < epsilon)) != false)
        return false;

    // Calculate = -1.0 / c4 * (m3 + c1 * m2 + c2 * m + c3 * I)
    __declspec(align(32)) Mat4x4 I;
    __declspec(align(32)) Mat4x4 tempA, tempB, tempC, tempD;

    Mat4x4SetI(I);
    Mat4x4MulScalar(tempA, I, c3);
    Mat4x4MulScalar(tempB, m, c2);
    Mat4x4MulScalar(tempC, m2, c1);

    Mat4x4Add(tempD, tempA, tempB);
    Mat4x4Add(tempD, tempD, tempC);
    Mat4x4Add(tempD, tempD, m3);
    Mat4x4MulScalar(m_inv, tempD, -1.0f / c4);
    return true;
}

void Avx64Mat4x4Inv(Mat4x4 m, const char* s)
{
    Mat4x4Printf(m, s);

    for (int i = 0; i <= 1; i++)
    {
        const float epsilon = 1.0e-9f;
        __declspec(align(32)) Mat4x4 m_inv;
        __declspec(align(32)) Mat4x4 m_ver;
        bool rc, is_singular;

        if (i == 0)
        {
            printf("\nCalculating inverse matrix - Mat4x4InvCpp\n");
            rc = Mat4x4InvCpp(m_inv, m, epsilon, &is_singular);
        }
        else
        {
            printf("\nCalculating inverse matrix - Mat4x4Inv_\n");
            rc = Mat4x4Inv_(m_inv, m, epsilon, &is_singular);
        }

        if (!rc)
        {
            if (is_singular)
                printf("Matrix 'm' is singular\n");
            else
                printf("Error occurred during calculation of matrix inverse\n");
        }
        else
        {
          Mat4x4Printf(m_inv, "\nInverse matrix\n");
          Mat4x4Mul(m_ver, m_inv, m);
          Mat4x4Printf(m_ver, "\nInverse matrix verification\n");
        }
    }
}

void Avx64CalcMat4x4Inv(void)
{
    __declspec(align(32)) Mat4x4 m;

    printf("Results for Avx64CalcMat4x4Inv\n");

    Mat4x4SetRow(m, 0, 2, 7, 3, 4);
    Mat4x4SetRow(m, 1, 5, 9, 6, 4.75);
    Mat4x4SetRow(m, 2, 6.5, 3, 4, 10);
    Mat4x4SetRow(m, 3, 7, 5.25, 8.125, 6);
    Avx64Mat4x4Inv(m, "\nTest Matrix #1\n");

    Mat4x4SetRow(m, 0, 0.5, 12, 17.25, 4);
    Mat4x4SetRow(m, 1, 5, 2, 6.75, 8);
    Mat4x4SetRow(m, 2, 13.125, 1, 3, 9.75);
    Mat4x4SetRow(m, 3, 16, 1.625, 7, 0.25);
    Avx64Mat4x4Inv(m, "\nTest Matrix #2\n");

    Mat4x4SetRow(m, 0, 2, 0, 0, 1);
    Mat4x4SetRow(m, 1, 0, 4, 5, 0);
    Mat4x4SetRow(m, 2, 0, 0, 0, 7);
    Mat4x4SetRow(m, 3, 0, 0, 0, 6);
    Avx64Mat4x4Inv(m, "\nTest Matrix #3\n");
}

int _tmain(int argc, _TCHAR* argv[])
{
#ifdef _DEBUG
    Avx64CalcMat4x4InvTest();
#endif
    Avx64CalcMat4x4Inv();
    Avx64CalcMat4x4InvTimed();
    return 0;
}
