        .model flat,c
        .const
AbsMask qword 7fffffffffffffffh, 7fffffffffffffffh
        .code

; extern "C" void AvxSfpArithmetic_(double a, double b, double results[8]);
;
; Description:  The following function demonstrates how to use basic
;               scalar DPFP arithmetic instructions.
;
; Requires:     AVX

AvxSfpArithmetic_ proc
        push ebp
        mov ebp,esp

; Load argument values
        mov eax,[ebp+24]                    ;eax = ptr to results array
        vmovsd xmm0,real8 ptr [ebp+8]       ;xmm0 = a
        vmovsd xmm1,real8 ptr [ebp+16]      ;xmm1 = b

; Perform basic arithmetic using AVX scalar DPFP instructions
        vaddsd xmm2,xmm0,xmm1               ;xmm2 = a + b
        vsubsd xmm3,xmm0,xmm1               ;xmm3 = a - b
        vmulsd xmm4,xmm0,xmm1               ;xmm4 - a * b
        vdivsd xmm5,xmm0,xmm1               ;xmm5 = a / b
        vmovsd real8 ptr [eax+0],xmm2       ;save a + b
        vmovsd real8 ptr [eax+8],xmm3       ;save a - b
        vmovsd real8 ptr [eax+16],xmm4      ;save a * b
        vmovsd real8 ptr [eax+24],xmm5      ;save a / b

; Compute min(a, b), max(a, b), sqrt(a) and fabs(b)
        vminsd xmm2,xmm0,xmm1               ;xmm2 = min(a, b)
        vmaxsd xmm3,xmm0,xmm1               ;xmm3 = max(a, b)
        vsqrtsd xmm4,xmm0,xmm0              ;xmm4 = sqrt(a)
        vandpd xmm5,xmm1,xmmword ptr [AbsMask]  ;xmm5 = fabs(b)
        vmovsd real8 ptr [eax+32],xmm2      ;save min(a, b)
        vmovsd real8 ptr [eax+40],xmm3      ;save max(a, b)
        vmovsd real8 ptr [eax+48],xmm4      ;save sqrt(a)
        vmovsd real8 ptr [eax+56],xmm5      ;save trunc(sqrt(a))

        pop ebp
        ret
AvxSfpArithmetic_ endp
        end
