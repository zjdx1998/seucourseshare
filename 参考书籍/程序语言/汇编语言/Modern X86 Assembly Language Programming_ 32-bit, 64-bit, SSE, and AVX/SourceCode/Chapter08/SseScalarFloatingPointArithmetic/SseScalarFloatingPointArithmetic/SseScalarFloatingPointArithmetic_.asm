        .model flat,c
        .const

; Mask values for floating-point absolute values
                align 16
AbsMaskFloat    dword 7fffffffh,0ffffffffh,0ffffffffh,0ffffffffh
AbsMaskDouble   qword 7fffffffffffffffh,0ffffffffffffffffh
        .code

; extern "C" void SseSfpArithmeticFloat_(float a, float b, float c[8])
;
; Description:  The following function demonstrates basic arithmetic
;               operations using scalar single-precision floating-point
;               values.
;
; Requires      SSE

SseSfpArithmeticFloat_ proc
        push ebp
        mov ebp,esp

; Load argument values
        movss xmm0,real4 ptr [ebp+8]        ;xmm0 = a
        movss xmm1,real4 ptr [ebp+12]       ;xmm1 = b
        mov eax,[ebp+16]                    ;eax = c

; Perform single-precision arithmetic operations
        movss xmm2,xmm0
        addss xmm2,xmm1                     ;xmm2 = a + b
        movss real4 ptr [eax],xmm2

        movss xmm3,xmm0
        subss xmm3,xmm1                     ;xmm3 = a - b
        movss real4 ptr [eax+4],xmm3

        movss xmm4,xmm0
        mulss xmm4,xmm1                     ;xmm4 = a * b
        movss real4 ptr [eax+8],xmm4

        movss xmm5,xmm0
        divss xmm5,xmm1                     ;xmm5 = a / b
        movss real4 ptr [eax+12],xmm5

        movss xmm6,xmm0
        minss xmm6,xmm1                     ;xmm6 = min(a, b)
        movss real4 ptr [eax+16],xmm6

        movss xmm7,xmm0
        maxss xmm7,xmm1                     ;xmm7 = max(a, b)
        movss real4 ptr [eax+20],xmm7

        andps xmm1,[AbsMaskFloat]           ;xmm1 = fabs(b)
        movss real4 ptr [eax+24],xmm1

        sqrtss xmm0,xmm1                    ;xmm0 = sqrt(fabs(b))
        movss real4 ptr [eax+28],xmm0

        pop ebp
        ret
SseSfpArithmeticFloat_ endp

; extern "C" void SseSfpArithmeticDouble_(double a, double b, double c[8])
;
; Description:  The following function demonstrates basic arithmetic
;               operations using scalar double-precision floating-point
;               values.
;
; Requires      SSE2

SseSfpArithmeticDouble_ proc
        push ebp
        mov ebp,esp

; Load argument values
        movsd xmm0,real8 ptr [ebp+8]        ;xmm0 = a
        movsd xmm1,real8 ptr [ebp+16]       ;xmm1 = b
        mov eax,[ebp+24]                    ;eax = c

; Perform double-precision arithmetic operations
        movsd xmm2,xmm0
        addsd xmm2,xmm1                     ;xmm2 = a + b
        movsd real8 ptr [eax],xmm2

        movsd xmm3,xmm0
        subsd xmm3,xmm1                     ;xmm3 = a - b
        movsd real8 ptr [eax+8],xmm3

        movsd xmm4,xmm0
        mulsd xmm4,xmm1                     ;xmm4 = a * b
        movsd real8 ptr [eax+16],xmm4

        movsd xmm5,xmm0
        divsd xmm5,xmm1                     ;xmm5 = a / b
        movsd real8 ptr [eax+24],xmm5

        movsd xmm6,xmm0
        minsd xmm6,xmm1                     ;xmm6 = min(a, b)
        movsd real8 ptr [eax+32],xmm6

        movsd xmm7,xmm0
        maxsd xmm7,xmm1                     ;xmm7 = max(a, b)
        movsd real8 ptr [eax+40],xmm7

        andpd xmm1,[AbsMaskDouble]          ;xmm1 = fabs(b)
        movsd real8 ptr [eax+48],xmm1

        sqrtsd xmm0,xmm1                    ;xmm0 = sqrt(fabs(b))
        movsd real8 ptr [eax+56],xmm0

        pop ebp
        ret
SseSfpArithmeticDouble_ endp
        end
