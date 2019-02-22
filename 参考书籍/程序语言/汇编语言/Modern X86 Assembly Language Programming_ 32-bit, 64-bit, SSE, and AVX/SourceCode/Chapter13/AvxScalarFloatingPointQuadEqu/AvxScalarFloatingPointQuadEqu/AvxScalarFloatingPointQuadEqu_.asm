        .model flat,c
        .const
FpNegateMask    qword 8000000000000000h,0   ;mask to negate DPFP value
FpAbsMask       qword 7FFFFFFFFFFFFFFFh,-1  ;mask to compute fabs()
r8_0p0          real8 0.0
r8_2p0          real8 2.0
r8_4p0          real8 4.0
        .code

; extern "C" void AvxSfpQuadEqu_(const double coef[3], double roots[2], double epsilon, int* dis);
;
; Description:  The following function calculates the roots of a
;               quadratic equation using the quadratic formula.
;
; Requires:     AVX

AvxSfpQuadEqu_ proc
        push ebp
        mov ebp,esp

; Load argument values
        mov eax,[ebp+8]                     ;eax = ptr to coeff array
        mov ecx,[ebp+12]                    ;ecx = ptr to roots array
        mov edx,[ebp+24]                    ;edx = ptr to dis
        vmovsd xmm0,real8 ptr [eax]         ;xmm0 = a
        vmovsd xmm1,real8 ptr [eax+8]       ;xmm1 = b
        vmovsd xmm2,real8 ptr [eax+16]      ;xmm2 = c
        vmovsd xmm7,real8 ptr [ebp+16]      ;xmm7 = epsilon

; Make sure coefficient a is valid
        vandpd xmm6,xmm0,[FpAbsMask]        ;xmm2 = fabs(a)
        vcomisd xmm6,xmm7
        jb Error                            ;jump if fabs(a) < epsilon

; Compute intermediate values
        vmulsd xmm3,xmm1,xmm1               ;xmm3 = b * b
        vmulsd xmm4,xmm0,[r8_4p0]           ;xmm4 = 4 * a
        vmulsd xmm4,xmm4,xmm2               ;xmm4 = 4 * a * c
        vsubsd xmm3,xmm3,xmm4               ;xmm3 = b * b - 4 * a * c
        vmulsd xmm0,xmm0,[r8_2p0]           ;xmm0 = 2 * a
        vxorpd xmm1,xmm1,[FpNegateMask]     ;xmm1 = -b

; Test delta to determine root type
        vandpd xmm2,xmm3,[FpAbsMask]        ;xmm2 = fabs(delta)
        vcomisd xmm2,xmm7
        jb IdenticalRoots                   ;jump if fabs(delta) < epsilon
        vcomisd xmm3,[r8_0p0]
        jb ComplexRoots                     ;jump if delta < 0.0

; Distinct real roots
; r1 = (-b + sqrt(delta)) / 2 * a, r2 = (-b - sqrt(delta)) / 2 * a
        vsqrtsd xmm3,xmm3,xmm3              ;xmm3 = sqrt(delta)
        vaddsd xmm4,xmm1,xmm3               ;xmm4 = -b + sqrt(delta)
        vsubsd xmm5,xmm1,xmm3               ;xmm5 = -b - sqrt(delta)
        vdivsd xmm4,xmm4,xmm0               ;xmm4 = final r1
        vdivsd xmm5,xmm5,xmm0               ;xmm5 = final r2
        vmovsd real8 ptr [ecx],xmm4         ;save r1
        vmovsd real8 ptr [ecx+8],xmm5       ;save r2
        mov dword ptr [edx],1               ;*dis = 1
        jmp done

; Identical roots
; r1 = r2 = -b / 2 * a
IdenticalRoots:
        vdivsd xmm4,xmm1,xmm0               ;xmm4 = -b / 2 * a
        vmovsd real8 ptr [ecx],xmm4         ;save r1
        vmovsd real8 ptr [ecx+8],xmm4       ;save r2
        mov dword ptr [edx],0               ;*dis = 0
        jmp done

; Complex roots
;  real = -b / 2 * a, imag = sqrt(-delta) / 2 * a
;  final roots: r1 = (real, imag), r2 = (real, -imag)
ComplexRoots:
        vdivsd xmm4,xmm1,xmm0               ;xmm4 = -b / 2 * a
        vxorpd xmm3,xmm3,[FpNegateMask]     ;xmm3 = -delta
        vsqrtsd xmm3,xmm3,xmm3              ;xmm3 = sqrt(-delta)
        vdivsd xmm5,xmm3,xmm0               ;xmm5 = sqrt(-delta) / 2 * a
        vmovsd real8 ptr [ecx],xmm4         ;save real part
        vmovsd real8 ptr [ecx+8],xmm5       ;save imaginary part
        mov dword ptr [edx],-1              ;*dis = -1

Done:   pop ebp
        ret

Error:  mov dword ptr [edx],9999              ;*dis = 9999 (error code)
        pop ebp
        ret
AvxSfpQuadEqu_ endp
        end
