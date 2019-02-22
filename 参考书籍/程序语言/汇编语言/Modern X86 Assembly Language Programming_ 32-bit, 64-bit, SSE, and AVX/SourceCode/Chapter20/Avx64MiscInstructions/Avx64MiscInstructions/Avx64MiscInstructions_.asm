        include <Macrosx86-64.inc>

        .const
MaskVgatherdps  dword 80000000h,80000000h,80000000h,80000000h
                dword 80000000h,80000000h,80000000h,80000000h
MaskVgatherqps  dword 80000000h,80000000h,80000000h,80000000h
        .code

; extern "C" void Avx64GatherFloatIndx32_(float g[8], const float* x, Int32 indices[8]);
;
; Description:  The following function demonstrates use of the
;               vgatherdps instruction.
;
; Requires      X86-64, AVX2

Avx64GatherFloatIndx32_ proc
        vmovdqu ymm0,ymmword ptr [r8]
        vmovdqu ymm1,ymmword ptr [MaskVgatherdps]

        vgatherdps ymm2,[rdx+ymm0*4],ymm1   ;ymm2 = gathered SPFP values

        vmovups ymmword ptr [rcx],ymm2      ;save result
        vzeroupper
        ret
Avx64GatherFloatIndx32_ endp

; extern "C" void Avx64GatherFloatIndx64_(float g[4], const float* x, Int64 indices[4]);
;
; Description:  The following function demonstrates use of the
;               vgatherqps instruction.
;
; Requires      X86-64, AVX2

Avx64GatherFloatIndx64_ proc
        vmovdqu ymm0,ymmword ptr [r8]
        vmovdqu xmm1,xmmword ptr [MaskVgatherqps]

        vgatherqps xmm2,[rdx+ymm0*4],xmm1   ;xmm2 = gathered SPFP values

        vmovups xmmword ptr [rcx],xmm2      ;save result
        vzeroupper
        ret
Avx64GatherFloatIndx64_ endp

; extern "C" void Avx64FloatToHp_(Uint16 x_hp[8], float x1[8]);
;
; Desciption:   The following function converts an array of eight
;               SPFP values to HPFP.
;
; Requires      X86-64, AVX, F16C

Avx64FloatToHp_ proc
        vmovups ymm0,ymmword ptr [rdx]
        vcvtps2ph xmmword ptr [rcx],ymm0,00000100b  ;use round to nearest
        ret
Avx64FloatToHp_ endp

; extern "C" void Avx64HpToFloat_(float x[8], Uint16 x_hp[8]);
;
; Desciption:   The following function converts an array of eight
;               HPFP values to SPFP.
;
; Requires      X86-64, AVX, F16C

Avx64HpToFloat_ proc
        vcvtph2ps ymm0,xmmword ptr [rdx]
        vmovups ymmword ptr [rcx],ymm0
        ret
Avx64HpToFloat_ endp
        end
