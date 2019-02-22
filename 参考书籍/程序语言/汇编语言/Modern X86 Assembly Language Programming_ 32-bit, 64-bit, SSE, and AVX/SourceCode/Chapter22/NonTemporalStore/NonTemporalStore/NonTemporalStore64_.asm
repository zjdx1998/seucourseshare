IFDEF ASMX86_64
        .code

; _CalcResult64 Macro
;
; The following macro contains a simple calculating loop that is used
; to compare performance of the movaps and movntps instructions.

_CalcResult64 macro MovInstr

; Load and validate arguments
        test rcx,0fh
        jnz Error                           ;jump if c is not aligned
        test rdx,0fh
        jnz Error                           ;jump if a is not aligned
        test r8,0fh
        jnz Error                           ;jump if b is not aligned

        test r9d,r9d
        jle Error                           ;jump if n <= 0
        test r9d,3
        jnz Error                           ;jump if n % 4 != 0

; Calculate c[i] = sqrt(a[i] * a[i] + b[i] * b[i])
        xor eax,eax                         ;eax = common array offset
        align 16
@@:     movaps xmm0,xmmword ptr [rdx+rax]   ;xmm0 = values from a[]
        movaps xmm1,xmmword ptr [r8+rax]    ;xmm1 = values from b[]
        mulps xmm0,xmm0                     ;xmm0 = a[i] * a[i]
        mulps xmm1,xmm1                     ;xmm1 = b[i] * b[i]
        addps xmm0,xmm1                     ;xmm0 = sum
        sqrtps xmm0,xmm0                    ;xmm0 = final result
        MovInstr xmmword ptr [rcx+rax],xmm0 ;save final values to c

        add rax,16                          ;update offset
        sub r9d,4                           ;update counter
        jnz @B

        mov eax,1                           ;set success return code
        ret

Error:  xor eax,eax                         ;set error return code
        ret
        endm

;extern bool CalcResultA_(float* c, const float* a, const float* b, int n)
CalcResultA_ proc
        _CalcResult64 movaps
CalcResultA_ endp

;extern bool CalcResultB_(float* c, const float* a, const float* b, int n)
CalcResultB_ proc
        _CalcResult64 movntps
CalcResultB_ endp
ENDIF
        end
