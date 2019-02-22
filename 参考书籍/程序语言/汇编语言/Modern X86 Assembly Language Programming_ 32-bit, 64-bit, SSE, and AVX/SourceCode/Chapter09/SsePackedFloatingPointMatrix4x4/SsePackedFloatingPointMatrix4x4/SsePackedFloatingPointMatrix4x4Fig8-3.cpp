#include "stdafx.h"
#include "SsePackedFloatingPointMatrix4x4.h"

void TestFig8_3(void)
{
    __declspec(align(16)) Mat4x4 m_src1;
    __declspec(align(16)) Mat4x4 m_src2;
    __declspec(align(16)) Mat4x4 m_des;

    Mat4x4SetRow(m_src1, 0, 1, 0, 0, 0);
    Mat4x4SetRow(m_src1, 1, 0, 1, 0, 0);
    Mat4x4SetRow(m_src1, 2, 0, 0, 1, 0);
    Mat4x4SetRow(m_src1, 3, 0, 0, 0, 1);

    Mat4x4SetRow(m_src2, 0, 2, 7, 8, 3);
    Mat4x4SetRow(m_src2, 1, 11, 14, 16, 10);
    Mat4x4SetRow(m_src2, 2, 24, 21, 27, 29);
    Mat4x4SetRow(m_src2, 3, 31, 34, 38, 33);

    SsePfpMatrix4x4Multiply_(m_des, m_src1, m_src2);
}
