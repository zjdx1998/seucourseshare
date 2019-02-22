IFDEF ASMX86_32
        .model flat,c
        .code

; _CalcResult32 Macro
;
; The following macro contains a simple calculating loop that is used
; to compare performance of the movaps and movntps instructions.

_CalcResult32 macro MovInstr
        push ebp
        mov ebp,esp
        push ebx
        push edi

; Load and validate arguments
        mov edi,[ebp+8]                     ;edi = c
        test edi,0fh
        jnz Error                           ;jump if c is not aligned
        mov ebx,[ebp+12]                    ;ebx = a
        test ebx,0fh
        jnz Error                           ;jump if a is not aligned
        mov edx,[ebp+16]                    ;edx = b
        test edx,0fh
        jnz Error                           ;jump if b is not aligned

        mov ecx,[ebp+20]                    ;ecx = n
        test ecx,ecx
        jle Error                           ;jump if n <= 0
        test ecx,3
        jnz Error                           ;jump if n % 4 != 0

; Calculate c[i] = sqrt(a[i] * a[i] + b[i] * b[i])
        xor eax,eax                         ;eax = common array offset
        align 16
@@:     movaps xmm0,xmmword ptr [ebx+eax]   ;xmm0 = values from a[]
        movaps xmm1,xmmword ptr [edx+eax]   ;xmm1 = values from b[]
        mulps xmm0,xmm0                     ;xmm0 = a[i] * a[i]
        mulps xmm1,xmm1                     ;xmm1 = b[i] * b[i]
        addps xmm0,xmm1                     ;xmm0 = sum
        sqrtps xmm0,xmm0                    ;xmm0 = final result
        MovInstr xmmword ptr [edi+eax],xmm0 ;save final values to c

        add eax,16                          ;update offset
        sub ecx,4                           ;update counter
        jnz @B

        mov eax,1                           ;set success return code
        pop edi
        pop ebx
        pop ebp
        ret

Error:  xor eax,eax                         ;set error return code
        pop ebx
        pop ebp
        ret
        endm

;extern bool CalcResultA_(float* c, const float* a, const float* b, int n)
CalcResultA_ proc
        _CalcResult32 movaps
CalcResultA_ endp

;extern bool CalcResultB_(float* c, const float* a, const float* b, int n)
CalcResultB_ proc
        _CalcResult32 movntps
CalcResultB_ endp
ENDIF
        end
