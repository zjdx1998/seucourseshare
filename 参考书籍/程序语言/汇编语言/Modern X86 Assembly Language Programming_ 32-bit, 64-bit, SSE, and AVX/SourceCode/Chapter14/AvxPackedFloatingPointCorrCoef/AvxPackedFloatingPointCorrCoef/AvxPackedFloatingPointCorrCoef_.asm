        .model flat,c
        .code
        extern CcEpsilon:real8

; extern "C" bool AvxPfpCorrCoef_(const double* x, const double* y, int n, double sums[5], double* rho);
;
; Description:  The following function computes the correlation
;               coeficient for the specified x and y arrays.
;
; Requires:     AVX

AvxPfpCorrCoef_ proc
        push ebp
        mov ebp,esp

; Load and validate argument values
        mov eax,[ebp+8]                     ;eax = ptr to x
        test eax,1fh
        jnz BadArg                          ;jump if x is not aligned
        mov edx,[ebp+12]                    ;edx = ptr to y
        test edx,1fh
        jnz BadArg                          ;jump if y is not aligned

        mov ecx,[ebp+16]                    ;ecx = n
        cmp ecx,4
        jl BadArg                           ;jump if n < 4
        test ecx,3                          ;is n evenly divisible by 4?
        jnz BadArg                          ;jump if no
        shr ecx,2                           ;ecx = num iterations

; Initialize sum variables to zero
        vxorpd ymm3,ymm3,ymm3               ;ymm3 = packed sum_x
        vmovapd ymm4,ymm3                   ;ymm4 = packed sum_y
        vmovapd ymm5,ymm3                   ;ymm5 = packed sum_xx
        vmovapd ymm6,ymm3                   ;ymm6 = packed sum_yy
        vmovapd ymm7,ymm3                   ;ymm7 = packed sum_xy

; Calculate intermediate packed sum variables
@@:     vmovapd ymm0,ymmword ptr [eax]      ;ymm0 = packed x values
        vmovapd ymm1,ymmword ptr [edx]      ;ymm1 = packed y values

        vaddpd ymm3,ymm3,ymm0               ;update packed sum_x
        vaddpd ymm4,ymm4,ymm1               ;update packed sum_y

        vmulpd ymm2,ymm0,ymm1               ;ymm2 = packed xy values
        vaddpd ymm7,ymm7,ymm2               ;update packed sum_xy

        vmulpd ymm0,ymm0,ymm0               ;ymm0 = packed xx values
        vmulpd ymm1,ymm1,ymm1               ;ymm1 = packed yy values
        vaddpd ymm5,ymm5,ymm0               ;update packed sum_xx
        vaddpd ymm6,ymm6,ymm1               ;update packed sum_yy

        add eax,32                          ;update x ptr
        add edx,32                          ;update y ptr
        dec ecx                             ;update loop counter
        jnz @B                              ;repeat if not finished

; Calculate final sum variables
        vextractf128 xmm0,ymm3,1
        vaddpd xmm1,xmm0,xmm3
        vhaddpd xmm3,xmm1,xmm1              ;xmm3[63:0] = sum_x

        vextractf128 xmm0,ymm4,1
        vaddpd xmm1,xmm0,xmm4
        vhaddpd xmm4,xmm1,xmm1              ;xmm4[63:0] = sum_y

        vextractf128 xmm0,ymm5,1
        vaddpd xmm1,xmm0,xmm5
        vhaddpd xmm5,xmm1,xmm1              ;xmm5[63:0] = sum_xx

        vextractf128 xmm0,ymm6,1
        vaddpd xmm1,xmm0,xmm6
        vhaddpd xmm6,xmm1,xmm1              ;xmm6[63:0] = sum_yy

        vextractf128 xmm0,ymm7,1
        vaddpd xmm1,xmm0,xmm7
        vhaddpd xmm7,xmm1,xmm1              ;xmm7[63:0] = sum_xy

; Save final sum variables
        mov eax,[ebp+20]                    ;eax = ptr to sums array
        vmovsd real8 ptr [eax],xmm3         ;save sum_x
        vmovsd real8 ptr [eax+8],xmm4       ;save sum_y
        vmovsd real8 ptr [eax+16],xmm5      ;save sum_xx
        vmovsd real8 ptr [eax+24],xmm6      ;save sum_yy
        vmovsd real8 ptr [eax+32],xmm7      ;save sum_xy

; Calculate rho numerator
; rho_num = n * sum_xy - sum_x * sum_y;
        vcvtsi2sd xmm2,xmm2,dword ptr [ebp+16]  ;xmm2 = n
        vmulsd xmm0,xmm2,xmm7                   ;xmm0= = n * sum_xy
        vmulsd xmm1,xmm3,xmm4                   ;xmm1 = sum_x * sum_y
        vsubsd xmm7,xmm0,xmm1                   ;xmm7 = rho_num

; Calculate rho denominator
; t1 = sqrt(n * sum_xx - sum_x * sum_x)
; t2 = sqrt(n * sum_yy - sum_y * sum_y)
; rho_den = t1 * t2
        vmulsd xmm0,xmm2,xmm5       ;xmm0 = n * sum_xx
        vmulsd xmm3,xmm3,xmm3       ;xmm3 = sum_x * sum_x
        vsubsd xmm3,xmm0,xmm3       ;xmm3 = n * sum_xx - sum_x * sum_x
        vsqrtsd xmm3,xmm3,xmm3      ;xmm3 = t1

        vmulsd xmm0,xmm2,xmm6       ;xmm0 = n * sum_yy
        vmulsd xmm4,xmm4,xmm4       ;xmm4 = sum_y * sum_y
        vsubsd xmm4,xmm0,xmm4       ;xmm4 = n * sum_yy - sum_y * sum_y
        vsqrtsd xmm4,xmm4,xmm4      ;xmm4 = t2

        vmulsd xmm0,xmm3,xmm4       ;xmm0 = rho_den

; Calculate final value of rho
        xor eax,eax                         ;clear upper bits of eax
        vcomisd xmm0,[CcEpsilon]            ;is rho_den < CcEpsilon?
        setae al                            ;set return code
        jb BadRho                           ;jump if rho_den < CcEpsilon

        vdivsd xmm1,xmm7,xmm0               ;xmm1 = rho
SvRho:  mov edx,[ebp+24]                    ;eax = ptr to rho
        vmovsd real8 ptr [edx],xmm1         ;save rho

        vzeroupper
Done:   pop ebp
        ret

; Error handlers
BadRho: vxorpd xmm1,xmm1,xmm1               ;rho = 0
        jmp SvRho
BadArg: xor eax,eax                         ;eax = invalid arg ret code
        jmp Done

AvxPfpCorrCoef_ endp
        end
