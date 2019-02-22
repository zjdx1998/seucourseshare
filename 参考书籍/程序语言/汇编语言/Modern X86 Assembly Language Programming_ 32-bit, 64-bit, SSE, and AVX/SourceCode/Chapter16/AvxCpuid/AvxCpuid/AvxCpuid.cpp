#include "stdafx.h"
#include "MiscDefs.h"
#include <memory.h>

// This structure is used to save cpuid instruction results. It must
// match the structure that's defined in AvxCpuid_.asm.
typedef struct
{
    Uint32 EAX;
    Uint32 EBX;
    Uint32 ECX;
    Uint32 EDX;
} CpuidRegs;

// This structure contains status flags for cpuid reportable features
// used in this book.
typedef struct
{
    // General information
    Uint32 MaxEAX;      // Maximum EAX value supported by cpuid
    char VendorId[13];  // Processor vendor id string

    // Processor feature flags. Set to 'true' if feature extension
    // or instruction group is available for use.
    bool SSE;
    bool SSE2;
    bool SSE3;
    bool SSSE3;
    bool SSE4_1;
    bool SSE4_2;
    bool AVX;
    bool AVX2;
    bool F16C;
    bool FMA;
    bool POPCNT;
    bool BMI1;
    bool BMI2;
    bool LZCNT;
    bool MOVBE;

    // OS enabled feature information
    bool OSXSAVE;       // True if XSAVE feature set is enabled by the OS
    bool SSE_STATE;     // True if XMM state is enabled by the OS
    bool AVX_STATE;     // True if YMM state is enabled by the OS
} CpuidFeatures;

extern "C" Uint32 Cpuid_(Uint32 r_eax, Uint32 r_ecx, CpuidRegs* out);
extern "C" void Xgetbv_(Uint32 r_ecx, Uint32* r_eax, Uint32* r_edx);

// This function will not work on older CPUs, especially
// those introduced before 2006. It has been tested using
// only Windows 7 (SP1) and Windows 8.1.

void GetCpuidFeatures(CpuidFeatures* cf)
{
    CpuidRegs r_out;

    memset(cf, 0, sizeof(CpuidFeatures));

    // Get MaxEAX and VendorID
    Cpuid_(0, 0, &r_out);
    cf->MaxEAX = r_out.EAX;
    *(Uint32 *)(cf->VendorId + 0) = r_out.EBX;
    *(Uint32 *)(cf->VendorId + 4) = r_out.EDX;
    *(Uint32 *)(cf->VendorId + 8) = r_out.ECX;
    cf->VendorId[12] = '\0';

    // Quit if processor is too old
    if (cf->MaxEAX < 10)
        return;

    // Get CPUID.01H feature flags
    Cpuid_(1, 0, &r_out);
    Uint32 cpuid01_ecx = r_out.ECX;
    Uint32 cpuid01_edx = r_out.EDX;

    // Get CPUID (EAX = 07H, ECX = 00H) feature flags
    Cpuid_(7, 0, &r_out);
    Uint32 cpuid07_ebx = r_out.EBX;

    // CPUID.01H:EDX.SSE[bit 25]
    cf->SSE = (cpuid01_edx & (0x1 << 25)) ? true : false;

    // CPUID.01H:EDX.SSE2[bit 26]
    if (cf->SSE)
        cf->SSE2 = (cpuid01_edx & (0x1 << 26)) ? true : false;

    // CPUID.01H:ECX.SSE3[bit 0]
    if (cf->SSE2)
        cf->SSE3 = (cpuid01_ecx & (0x1 << 0)) ? true : false;

    // CPUID.01H:ECX.SSSE3[bit 9]
    if (cf->SSE3)
        cf->SSSE3 = (cpuid01_ecx & (0x1 << 9)) ? true : false;

    // CPUID.01H:ECX.SSE4.1[bit 19]
    if (cf->SSSE3)
        cf->SSE4_1 = (cpuid01_ecx & (0x1 << 19)) ? true : false;

    // CPUID.01H:ECX.SSE4.2[bit 20]
    if (cf->SSE4_1)
        cf->SSE4_2 = (cpuid01_ecx & (0x1 << 20)) ? true : false;

    // CPUID.01H:ECX.POPCNT[bit 23]
    if (cf->SSE4_2)
        cf->POPCNT = (cpuid01_ecx & (0x1 << 23)) ? true : false;

    // CPUID.01H:ECX.OSXSAVE[bit 27]
    cf->OSXSAVE = (cpuid01_ecx & (0x1 << 27)) ? true : false;

    // Test OSXSAVE status to verify XGETBV is enabled
    if (cf->OSXSAVE)
    {
        // Use XGETBV to obtain following information
        //  XSAVE uses SSE state if (XCR0[1] == 1) is true
        //  XSAVE uses AVX state if (XCR0[2] == 1) is true

        Uint32 xgetbv_eax, xgetbv_edx;

        Xgetbv_(0, &xgetbv_eax, &xgetbv_edx);
        cf->SSE_STATE = (xgetbv_eax & (0x1 << 1)) ? true : false;
        cf->AVX_STATE = (xgetbv_eax & (0x1 << 2)) ? true : false;

        // Is SSE and AVX state information supported by the OS?
        if (cf->SSE_STATE && cf->AVX_STATE)
        {
            // CPUID.01H:ECX.AVX[bit 28] = 1
            cf->AVX = (cpuid01_ecx & (0x1 << 28)) ? true : false;

            if (cf->AVX)
            {
                // CPUID.01H:ECX.F16C[bit 29]
                cf->F16C = (cpuid01_ecx & (0x1 << 29)) ? true : false;

                // CPUID.01H:ECX.FMA[bit 12]
                cf->FMA = (cpuid01_ecx & (0x1 << 12)) ? true : false;

                // CPUID.(EAX = 07H, ECX = 00H):EBX.AVX2[bit 5]
                cf->AVX2 = (cpuid07_ebx & (0x1 << 5)) ? true : false;
            }
        }
    }

    // CPUID.(EAX = 07H, ECX = 00H):EBX.BMI1[bit 3]
    cf->BMI1 = (cpuid07_ebx & (0x1 << 3)) ? true : false;

    // CPUID.(EAX = 07H, ECX = 00H):EBX.BMI2[bit 8]
    cf->BMI2 = (cpuid07_ebx & (0x1 << 8)) ? true : false;

    // CPUID.80000001H:ECX.LZCNT[bit 5]
    Cpuid_(0x80000001, 0, &r_out);
    cf->LZCNT = (r_out.ECX & (0x1 << 5)) ? true : false;

    // Get MOVBE
    // CPUID.01H:ECX.MOVBE[bit 22]
    cf->MOVBE = cpuid01_ecx & (0x1 << 22) ? true : false;
}

int _tmain(int argc, _TCHAR* argv[])
{
    CpuidFeatures cf;

    GetCpuidFeatures(&cf);
    printf("Results for AvxCpuid\n");
    printf("MaxEAX:    %d\n", cf.MaxEAX);
    printf("VendorId:  %s\n", cf.VendorId);
    printf("SSE:       %d\n", cf.SSE);
    printf("SSE2:      %d\n", cf.SSE2);
    printf("SSE3:      %d\n", cf.SSE3);
    printf("SSSE3:     %d\n", cf.SSSE3);
    printf("SSE4_1:    %d\n", cf.SSE4_1);
    printf("SSE4_2:    %d\n", cf.SSE4_2);
    printf("POPCNT:    %d\n", cf.POPCNT);
    printf("AVX:       %d\n", cf.AVX);
    printf("F16C:      %d\n", cf.F16C);
    printf("FMA:       %d\n", cf.FMA);
    printf("AVX2:      %d\n", cf.AVX2);
    printf("BMI1       %d\n", cf.BMI1);
    printf("BMI2       %d\n", cf.BMI2);
    printf("LZCNT      %d\n", cf.LZCNT);
    printf("MOVBE      %d\n", cf.MOVBE);
    printf("\n");
    printf("OSXSAVE    %d\n", cf.OSXSAVE);
    printf("SSE_STATE  %d\n", cf.SSE_STATE);
    printf("AVX_STATE  %d\n", cf.AVX_STATE);

    return 0;
}
