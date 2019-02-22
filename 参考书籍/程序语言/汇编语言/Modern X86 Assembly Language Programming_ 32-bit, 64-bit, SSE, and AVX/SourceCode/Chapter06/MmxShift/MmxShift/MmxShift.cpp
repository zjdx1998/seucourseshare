#include "stdafx.h"
#include "MmxVal.h"

// The order of the name constants in the following enum must
// correspond to the table that is defined in MmxShift_.asm.

enum MmxShiftOp : unsigned int
{
    psllw,      // shift left logical word
    psrlw,      // shift right logical word
    psraw,      // shift right arithmetic word
    pslld,      // shift left logical dword
    psrld,      // shift right logical dword
    psrad,      // shift right arithmetic dword
};

extern "C" bool MmxShift_(MmxVal a, MmxShiftOp shift_op, int count, MmxVal* b);

void MmxShiftWords(void)
{
    MmxVal a, b;
    int count;
    char buff[256];

    a.u16[0] = 0x1234;
    a.u16[1] = 0xFF00;
    a.u16[2] = 0x00CC;
    a.u16[3] = 0x8080;
    count = 2;

    MmxShift_(a, MmxShiftOp::psllw, count, &b);
    printf("\nResults for psllw - count = %d\n", count);
    printf("a: %s\n", a.ToString_x16(buff, sizeof(buff)));
    printf("b: %s\n", b.ToString_x16(buff, sizeof(buff)));

    MmxShift_(a, MmxShiftOp::psrlw, count, &b);
    printf("\nResults for psrlw - count = %d\n", count);
    printf("a: %s\n", a.ToString_x16(buff, sizeof(buff)));
    printf("b: %s\n", b.ToString_x16(buff, sizeof(buff)));

    MmxShift_(a, MmxShiftOp::psraw, count, &b);
    printf("\nResults for psraw - count = %d\n", count);
    printf("a: %s\n", a.ToString_x16(buff, sizeof(buff)));
    printf("b: %s\n", b.ToString_x16(buff, sizeof(buff)));
}

void MmxShiftDwords(void)
{
    MmxVal a, b;
    int count;
    char buff[256];

    a.u32[0] = 0x00010001;
    a.u32[1] = 0x80008000;
    count = 3;

    MmxShift_(a, MmxShiftOp::pslld, count, &b);
    printf("\nResults for pslld - count = %d\n", count);
    printf("a: %s\n", a.ToString_x32(buff, sizeof(buff)));
    printf("b: %s\n", b.ToString_x32(buff, sizeof(buff)));

    MmxShift_(a, MmxShiftOp::psrld, count, &b);
    printf("\nResults for psrld - count = %d\n", count);
    printf("a: %s\n", a.ToString_x32(buff, sizeof(buff)));
    printf("b: %s\n", b.ToString_x32(buff, sizeof(buff)));

    MmxShift_(a, MmxShiftOp::psrad, count, &b);
    printf("\nResults for psrad - count = %d\n", count);
    printf("a: %s\n", a.ToString_x32(buff, sizeof(buff)));
    printf("b: %s\n", b.ToString_x32(buff, sizeof(buff)));
}

int _tmain(int argc, _TCHAR* argv[])
{
    MmxShiftWords();
    MmxShiftDwords();
    return 0;
}
