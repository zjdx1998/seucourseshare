        .model flat,c
        extern LsEpsilon:real8
        .const
PackedFp64Abs qword 7fffffffffffffffh,7fffffffffffffffh
        .code

; extern "C" bool SsePfpLeastSquares_(const double* x, const double* y, int n, double* m, double* b);
;
; Description:  The following function computes the slope and intercept
;               of a least squares regression line.
;
; Returns       0 = error (invalid n or improperly aligned array)
;               1 = success
;
; Requires:     SSE3

SsePfpLeastSquares_ proc
        push ebp
        mov ebp,esp
        push ebx

; Load and validate arguments
        xor eax,eax                         ;set error return code
        mov ebx,[ebp+8]                     ;ebx = 'x'
        test ebx,0fh
        jnz Done                            ;jump if 'x' not aligned
        mov edx,[ebp+12]                    ;edx ='y'
        test edx,0fh
        jnz Done                            ;jump if 'y' not aligned
        mov ecx,[ebp+16]                    ;ecx = n
        cmp ecx,2
        jl Done                             ;jump if n < 2

; Initialize sum registers
        cvtsi2sd xmm3,ecx                   ;xmm3 = DPFP n
        mov eax,ecx
        and ecx,0fffffffeh                  ;ecx = n / 2 * 2
        and eax,1                           ;eax = n % 2

        xorpd xmm4,xmm4                     ;sum_x (both qwords)
        xorpd xmm5,xmm5                     ;sum_y (both qwords)
        xorpd xmm6,xmm6                     ;sum_xx (both qwords)
        xorpd xmm7,xmm7                     ;sum_xy (both qwords)

; Calculate sum variables. Note that two values are processed each cycle.
@@:     movapd xmm0,xmmword ptr [ebx]       ;load next two x values
        movapd xmm1,xmmword ptr [edx]       ;load next two y values
        movapd xmm2,xmm0                    ;copy of x

        addpd xmm4,xmm0                     ;update sum_x
        addpd xmm5,xmm1                     ;update sum_y
        mulpd xmm0,xmm0                     ;calc x * x
        addpd xmm6,xmm0                     ;update sum_xx
        mulpd xmm2,xmm1                     ;calc x * y
        addpd xmm7,xmm2                     ;update sum_xy

        add ebx,16                          ;ebx = next x array value
        add edx,16                          ;edx = next x array value
        sub ecx,2                           ;adjust counter
        jnz @B                              ;repeat until done

; Update sum variables with final x, y values if 'n' is odd
        or eax,eax
        jz CalcFinalSums                    ;jump if n is even
        movsd xmm0,real8 ptr [ebx]          ;load final x
        movsd xmm1,real8 ptr [edx]          ;load final y
        movsd xmm2,xmm0

        addsd xmm4,xmm0                     ;update sum_x
        addsd xmm5,xmm1                     ;update sum_y
        mulsd xmm0,xmm0                     ;calc x * x
        addsd xmm6,xmm0                     ;update sum_xx
        mulsd xmm2,xmm1                     ;calc x * y
        addsd xmm7,xmm2                     ;update sum_xy

; Calculate final sum values
CalcFinalSums:
        haddpd xmm4,xmm4                    ;xmm4[63:0] = final sum_x
        haddpd xmm5,xmm5                    ;xmm5[63:0] = final sum_y
        haddpd xmm6,xmm6                    ;xmm6[63:0] = final sum_xx
        haddpd xmm7,xmm7                    ;xmm7[63:0] = final sum_xy

; Compute denom and make sure it's valid
; denom = n * sum_xx - sum_x * sum_x
        movsd xmm0,xmm3                     ;n
        movsd xmm1,xmm4                     ;sum_x
        mulsd xmm0,xmm6                     ;n * sum_xx
        mulsd xmm1,xmm1                     ;sum_x * sum_x
        subsd xmm0,xmm1                     ;xmm0 = denom
        movsd xmm2,xmm0
        andpd xmm2,xmmword ptr [PackedFp64Abs]  ;xmm2 = fabs(denom)
        comisd xmm2,real8 ptr [LsEpsilon]
        jb BadDenom                          ;jump if denom < fabs(denom)

; Compute and save slope
; slope = (n * sum_xy - sum_x * sum_y) / denom
        movsd xmm1,xmm4                     ;sum_x
        mulsd xmm3,xmm7                     ;n * sum_xy
        mulsd xmm1,xmm5                     ;sum_x * sum_y
        subsd xmm3,xmm1                     ;slope_numerator
        divsd xmm3,xmm0                     ;xmm3 = final slope
        mov edx,[ebp+20]                    ;edx = 'm'
        movsd real8 ptr [edx],xmm3          ;save slope

; Compute and save intercept
; intercept = (sum_xx * sum_y - sum_x * sum_xy) / denom
        mulsd xmm6,xmm5                     ;sum_xx * sum_y
        mulsd xmm4,xmm7                     ;sum_x * sum_xy
        subsd xmm6,xmm4                     ;intercept_numerator
        divsd xmm6,xmm0                     ;xmm6 = final intercept
        mov edx,[ebp+24]                    ;edx = 'b'
        movsd real8 ptr [edx],xmm6          ;save intercept
        mov eax,1                           ;success return code

Done:   pop ebx
        pop ebp
        ret

; Set 'm' and 'b' to 0.0
BadDenom:
        xor eax,eax                             ;set error code
        mov edx,[ebp+20]                        ;eax = 'm'
        mov [edx],eax
        mov [edx+4],eax                         ;*m = 0.0
        mov edx,[ebp+24]                        ;edx = 'b'
        mov [edx],eax
        mov [edx+4],eax                         ;*b = 0.0
        jmp Done

SsePfpLeastSquares_ endp
        end
