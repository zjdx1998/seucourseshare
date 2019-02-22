        .model flat,c
        .const
        align 16

; Mask value for packed SPFP absolute value
AbsMask dword 7fffffffh,7fffffffh,7fffffffh,7fffffffh
        dword 7fffffffh,7fffffffh,7fffffffh,7fffffffh

; Mask value for packed SPFP negation
NegMask dword 80000000h,80000000h,80000000h,80000000h
        dword 80000000h,80000000h,80000000h,80000000h
        .code

; extern "C" void AvxPfpArithmeticFloat_(const YmmVal* a, const YmmVal* b, YmmVal c[6]);
;
; Description:  The following function illustrates how to use common
;               packed SPFP arithmetic instructions using the YMM
;               registers.
;
; Requires:     AVX

AvxPfpArithmeticFloat_ proc
        push ebp
        mov ebp,esp

; Load argument values.  Note that the vmovaps instruction
; requires proper aligment of operands in memory.
        mov eax,[ebp+8]                     ;eax = ptr to a
        mov ecx,[ebp+12]                    ;ecx = ptr to b
        mov edx,[ebp+16]                    ;edx = ptr to c
        vmovaps ymm0,ymmword ptr [eax]      ;ymm0 = a
        vmovaps ymm1,ymmword ptr [ecx]      ;ymm1 = b

; Perform packed SPFP addition, subtraction, multiplication,
; and division
        vaddps ymm2,ymm0,ymm1               ;a + b
        vmovaps ymmword ptr [edx],ymm2

        vsubps ymm3,ymm0,ymm1               ;a - b
        vmovaps ymmword ptr [edx+32],ymm3

        vmulps ymm4,ymm0,ymm1               ;a * b
        vmovaps ymmword ptr [edx+64],ymm4

        vdivps ymm5,ymm0,ymm1               ;a / b
        vmovaps ymmword ptr [edx+96],ymm5

; Compute packed SPFP absolute value
        vmovups ymm6,ymmword ptr [AbsMask]          ;ymm6 = AbsMask
        vandps ymm7,ymm0,ymm6                       ;ymm7 = packed fabs
        vmovaps ymmword ptr [edx+128],ymm7

; Compute packed SPFP negation
        vxorps ymm7,ymm0,ymmword ptr [NegMask]      ;ymm7 = packed neg.
        vmovaps ymmword ptr [edx+160],ymm7

; Zero upper 128-bit of all YMM registers to avoid potential x86-AVX
; to x86-SSE transition penalties.
        vzeroupper

        pop ebp
        ret
AvxPfpArithmeticFloat_ endp

; extern "C" void AvxPfpArithmeticDouble_(const YmmVal* a, const YmmVal* b, YmmVal c[5]);
;
; Description:  The following function illustrates how to use common
;               packed DPFP arithmetic instructions using the YMM
;               registers.
;
; Requires:     AVX


AvxPfpArithmeticDouble_ proc
        push ebp
        mov ebp,esp

; Load argument values.  Note that the vmovapd instruction
; requires proper aligment of operands in memory.
        mov eax,[ebp+8]                     ;eax = ptr to a
        mov ecx,[ebp+12]                    ;ecx = ptr to b
        mov edx,[ebp+16]                    ;edx = ptr to c
        vmovapd ymm0,ymmword ptr [eax]      ;ymm0 = a
        vmovapd ymm1,ymmword ptr [ecx]      ;ymm1 = b

; Compute packed min, max and square root
        vminpd ymm2,ymm0,ymm1
        vmaxpd ymm3,ymm0,ymm1
        vsqrtpd ymm4,ymm0

; Perform horizontal addition and subtraction
        vhaddpd ymm5,ymm0,ymm1
        vhsubpd ymm6,ymm0,ymm1

; Save the results
        vmovapd ymmword ptr [edx],ymm2
        vmovapd ymmword ptr [edx+32],ymm3
        vmovapd ymmword ptr [edx+64],ymm4
        vmovapd ymmword ptr [edx+96],ymm5
        vmovapd ymmword ptr [edx+128],ymm6

; Zero upper 128-bit of all YMM registers to avoid potential x86-AVX
; to x86-SSE transition penalties.
        vzeroupper

        pop ebp
        ret
AvxPfpArithmeticDouble_ endp
        end
