        .model flat,c
        .const
        public LsEpsilon_
LsEpsilon_ real8 1.0e-12                    ;epsilon for valid denom test
        .code

; extern "C" bool CalcLeastSquares_(const double* x, const double* y, int n, double* m, double* b);
;
; Description:  The following function computes the slope and intercept
;               of a least squares regression line.
;
; Returns       0 = error
;               1 = success

CalcLeastSquares_ proc
        push ebp
        mov ebp,esp
        sub esp,8                           ;space for denom

        xor eax,eax                         ;set error return code
        mov ecx,[ebp+16]                    ;n
        test ecx,ecx
        jle Done                            ;jump if n <= 0
        mov eax,[ebp+8]                     ;ptr to x
        mov edx,[ebp+12]                    ;ptr to y

; Initialize all sum variables to zero
        fldz                                ;sum_xx
        fldz                                ;sum_xy
        fldz                                ;sum_y
        fldz                                ;sum_x
;STACK: sum_x, sum_y, sum_xy, sum_xx

@@:     fld real8 ptr [eax]                 ;load next x
        fld st(0)
        fld st(0)
        fld real8 ptr [edx]                 ;load next y
;STACK: y, x, x, x, sum_x, sum_y, sum_xy, sum_xx

        fadd st(5),st(0)                    ;sum_y += y
        fmulp               
;STACK: xy, x, x, sum_xm sum_y, sum_xy, sum_xx

        faddp st(5),st(0)                   ;sum_xy += xy
;STACK: x, x, sum_x, sum_y, sum_xy, sum_xx

        fadd st(2),st(0)                    ;sum_x += x
        fmulp
;STACK: xx, sum_x, sum_y, sum_xy, sum_xx

        faddp st(4),st(0)                   ;sum_xx += xx
;STACK: sum_x, sum_y, sum_xy, sum_xx

; Update pointers and repeat until elements have been processed.
        add eax,8
        add edx,8
        dec ecx
        jnz @B

; Compute denom = n * sum_xx - sum_x * sum_x
        fild dword ptr [ebp+16]             ;n
        fmul st(0),st(4)                    ;n * sum_xx
;STACK: n * sum_xx, sum_x, sum_y, sum_xy, sum_xx

        fld st(1)
        fld st(0)
;STACK: sum_x, sum_x, n * sum_xx, sum_x, sum_y, sum_xy, sum_xx

        fmulp
        fsubp
        fst real8 ptr [ebp-8]           ;save denom
;STACK: denom, sum_x, sum_y, sum_xy, sum_xx

; Verify that denom is valid
        fabs                            ;fabs(denom)
        fld real8 ptr [LsEpsilon_]
        fcomip st(0),st(1)              ;compare epsilon and fabs(demon)
        fstp st(0)                      ;remove fabs(denom) from stack
        jae InvalidDenom                ;jump if LsEpsilon_ >= fabs(denom)
;STACK: sum_x, sum_y, sum_xy, sum_xx

; Compute slope = (n * sum_xy - sum_x * sum_y) / denom
        fild dword ptr [ebp+16]
;STACK: n, sum_x, sum_y, sum_xy, sum_xx

        fmul st(0),st(3)                ;n * sum_xy
        fld st(2)                       ;sum_y
        fld st(2)                       ;sum_x
        fmulp                           ;sum_x * sum_y
        fsubp                           ;n * sum_xy - sum_x * sum_y
        fdiv real8 ptr [ebp-8]          ;calculate slope
        mov eax,[ebp+20]
        fstp real8 ptr [eax]            ;save slope
;STACK: sum_x, sum_y, sum_xy, sum_xx

; Calculate intercept = (sum_xx * sum_y - sum_x * sum_xy) / denom
        fxch st(3)
;STACK: sum_xx, sum_y, sum_xy, sum_x

        fmulp
        fxch st(2)
;STACK: sum_x, sum_xy, sum_xx * sum_y

        fmulp
        fsubp
;STACK: sum_xx * sum_y - sum_x * sum_xy

        fdiv real8 ptr [ebp-8]          ;calculate intercept
        mov eax,[ebp+24]
        fstp real8 ptr [eax]            ;save intercept
        mov eax,1                       ;set success return code

Done:   mov esp,ebp
        pop ebp
        ret

InvalidDenom:
; Cleanup x87 FPU register stack
        fstp st(0)
        fstp st(0)
        fstp st(0)
        fstp st(0)
        xor eax,eax                     ;set error return code
        mov esp,ebp
        pop ebp
        ret
CalcLeastSquares_ endp
        end
