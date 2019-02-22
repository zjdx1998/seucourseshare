        include <MacrosX86-64.inc>
        extern pow:proc

        .const
r8_1p0   real8 1.0
r8_3p0   real8 3.0
r8_4p0   real8 4.0
r8_pi    real8 3.14159265358979323846
        .code

; extern "C" bool Avx64CalcEllipsoid_(const double* a, const double* b, const double* c, int n, double p, double* sa, double* vol);
;
; Description:  The following function calculates the surface area
;               and volume of an ellipsoid
;
; Requires:     x86-64, AVX

Avx64CalcEllipsoid_ proc frame
        _CreateFrame Ce_,0,144,rbx,rsi,rdi,r12,r13,r14,r15
        _SaveXmmRegs xmm6,xmm7,xmm8,xmm9,xmm10,xmm12,xmm13,xmm14,xmm15
        _EndProlog

; Perform required register initializations. Note that non-volatile
; registers are used since this function calls the function pow().
        test r9d,r9d                        ;is n <= 0?
        jle Error                           ;jump if yes
        mov r12,rcx                         ;r12 = a ptr
        mov r13,rdx                         ;r13 = b ptr
        mov r14,r8                          ;r14 = c ptr
        mov r15d,r9d                        ;r15 = n

        vmovsd xmm12,real8 ptr [rbp+Ce_OffsetStackArgs]   ;xmm12 = p
        vmovsd xmm0,real8 ptr [r8_1p0]
        vdivsd xmm13,xmm0,xmm12             ;xmm13 = 1 / p
        vmovsd xmm1,real8 ptr [r8_4p0]
        vmulsd xmm14,xmm1,[r8_pi]           ;xmm14 = 4 * pi
        vmovsd xmm15,[r8_3p0]               ;xmm15 = 3

        mov rsi,[rbp+Ce_OffsetStackArgs+8]  ;rsi = sa ptr
        mov rdi,[rbp+Ce_OffsetStackArgs+16] ;rdi = vol ptr
        xor rbx,rbx                         ;rbx = common array offset
        sub rsp,32                          ;allocate home area for pow()

@@:     vmovsd xmm6,real8 ptr [r12+rbx]     ;xmm6 = a
        vmovsd xmm7,real8 ptr [r13+rbx]     ;xmm7 = b
        vmovsd xmm8,real8 ptr [r14+rbx]     ;xmm8 = c

; Calculate the ellipsoid's volume
        vmulsd xmm0,xmm14,xmm6              ;xmm0 = 4 * pi * a
        vmulsd xmm1,xmm7,xmm8               ;xmm1 = b * c;
        vmulsd xmm0,xmm0,xmm1               ;xmm0 = 4 * pi * a * b * c
        vdivsd xmm0,xmm0,xmm15              ;xmm0 = 4 * pi * a * b * c / 3
        vmovsd real8 ptr [rdi+rbx],xmm0     ;save ellipsoid volume

; Calculate the ellipsoid's surface area (see text for equation)
        vmovsd xmm0,xmm0,xmm6               ;xmm0 = a
        vmovsd xmm1,xmm1,xmm12              ;xmm1 = p
        call pow
        vmovsd xmm9,xmm9,xmm0               ;xmm9 = pow(a,p)

        vmovsd xmm0,xmm0,xmm7               ;xmm0 = b
        vmovsd xmm1,xmm1,xmm12              ;xmm1 = p
        call pow
        vmovsd xmm10,xmm10,xmm0             ;xmm10 = pow(b,p)

        vmovsd xmm0,xmm0,xmm8               ;xmm0 = c
        vmovsd xmm1,xmm1,xmm12              ;xmm1 = p
        call pow                            ;xmm0 = pow(c,p)

        vmulsd xmm1,xmm9,xmm10              ;xmm1 = pow(a,p) * pow(b,p)
        vmulsd xmm2,xmm9,xmm0               ;xmm2 = pow(a,p) * pow(c,p)
        vmulsd xmm3,xmm10,xmm0              ;xmm3 = pow(b,p) * pow(c,p)

        vaddsd xmm0,xmm1,xmm2
        vaddsd xmm0,xmm0,xmm3
        vdivsd xmm0,xmm0,xmm15              ;xmm0 = bracket sub expression
        vmovsd xmm1,xmm1,xmm13              ;xmm1 = 1 / p
        call pow                            ;xmm0 = pow(subexpr,1/p)
        vmulsd xmm0,xmm0,xmm14              ;xmm0 = final surface area
        vmovsd real8 ptr [rsi+rbx],xmm0     ;save surface area

; Update the counter and offset value, repeat if not finished
        add rbx,8
        sub r15,1
        jnz @B
        mov eax,1                           ;set success return code

Done:   _RestoreXmmRegs xmm6,xmm7,xmm8,xmm9,xmm10,xmm12,xmm13,xmm14,xmm15
        _DeleteFrame rbx,rsi,rdi,r12,r13,r14,r15
        ret

Error:  xor eax,eax                         ;set error return code
        jmp done
Avx64CalcEllipsoid_ endp
        end
