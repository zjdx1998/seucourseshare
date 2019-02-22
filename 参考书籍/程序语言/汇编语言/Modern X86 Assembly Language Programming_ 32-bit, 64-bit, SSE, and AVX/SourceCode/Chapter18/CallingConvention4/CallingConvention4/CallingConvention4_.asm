        include <MacrosX86-64.inc>

; Floating-point constants for BSA equations
                .const
r8_0p007184     real8 0.007184
r8_0p725        real8 0.725
r8_0p425        real8 0.425
r8_0p0235       real8 0.0235
r8_0p42246      real8 0.42246
r8_0p51456      real8 0.51456
r8_60p0         real8 60.0

        .code
        extern pow:proc

; extern "C" bool Cc4_(const double* ht, const double* wt, int n, double* bsa1, double* bsa2, double* bsa3);
;
; Description:  The following function demonstrates use of the macros
;               _CreateFrame, _DeleteFrame, _EndProlog, _SaveXmmRegs,
;               and _RestoreXmmRegs.

Cc4_    proc frame
        _CreateFrame Cc4_,16,64,rbx,rsi,r12,r13,r14,r15
        _SaveXmmRegs xmm6,xmm7,xmm8,xmm9
        _EndProlog

; Save argument registers to home area (optional). Note that the home
; area can also be used to store other transient data values.
        mov qword ptr [rbp+Cc4_OffsetHomeRCX],rcx
        mov qword ptr [rbp+Cc4_OffsetHomeRDX],rdx
        mov qword ptr [rbp+Cc4_OffsetHomeR8],r8
        mov qword ptr [rbp+Cc4_OffsetHomeR9],r9

; Initialize processing loop pointers.  Note that the pointers are
; maintained in non-volatile registers, which eliminates reloads
; after calls to pow().
        test r8d,r8d                            ;is n <= 0?
        jle Error                               ;jump if n <= 0
        mov [rbp],r8d                           ;save n to local var

        mov r12,rcx                             ;r12 = ptr to ht
        mov r13,rdx                             ;r13 = ptr to wt
        mov r14,r9                              ;r14 = ptr to bsa1
        mov r15,[rbp+Cc4_OffsetStackArgs]       ;r15 = ptr to bsa2
        mov rbx,[rbp+Cc4_OffsetStackArgs+8]     ;rbx = ptr to bsa3
        xor rsi,rsi                             ;array element offset

; Allocate home space on stack for use by pow()
        sub rsp,32

; Calculate bsa1 = 0.007184 * pow(ht, 0.725) * pow(wt, 0.425);
@@:     movsd xmm0,real8 ptr [r12+rsi]          ;xmm0 = height
        movsd xmm8,xmm0
        movsd xmm1,real8 ptr [r8_0p725]
        call pow                                ;xmm0 = pow(ht,0.725)
        movsd xmm6,xmm0

        movsd xmm0,real8 ptr [r13+rsi]          ;xmm0 = weight
        movsd xmm9,xmm0
        movsd xmm1,real8 ptr [r8_0p425]
        call pow                                ;xmm0 = pow(wt,0.425)
        mulsd xmm6,real8 ptr [r8_0p007184]
        mulsd xmm6,xmm0                         ;xmm6 = bsa1

; Calculate bsa2 = 0.0235 * pow(ht, 0.42246) * pow(wt, 0.51456);
        movsd xmm0,xmm8                         ;xmm0 = height
        movsd xmm1,real8 ptr [r8_0p42246]
        call pow                                ;xmm0 = pow(ht,0.42246)
        movsd xmm7,xmm0

        movsd xmm0,xmm9                         ;xmm0 = weight
        movsd xmm1,real8 ptr [r8_0p51456]
        call pow                                ;xmm0 = pow(wt,0.51456)
        mulsd xmm7,real8 ptr [r8_0p0235]
        mulsd xmm7,xmm0                         ;xmm7 = bsa2

; Calculate bsa3 = sqrt(ht * wt) / 60.0;
        mulsd xmm8,xmm9
        sqrtsd xmm8,xmm8
        divsd xmm8,real8 ptr [r8_60p0]          ;xmm8 = bsa3

; Save BSA results
        movsd real8 ptr [r14+rsi],xmm6          ;save bsa1 result
        movsd real8 ptr [r15+rsi],xmm7          ;save bsa2 result
        movsd real8 ptr [rbx+rsi],xmm8          ;save bsa3 result

        add rsi,8                               ;update array offset
        dec dword ptr [rbp]                     ;n = n - 1
        jnz @B
        mov eax,1                               ;set success return code

; Restore all used non-volatile XMM and GP registers.  Note that the 
; _DeleteFrame macro restores rsp from rbp, which means that it is not
; necessary to include an explicit add rsp,32 instruction to "free"
; the pow() home area.
Done:   _RestoreXmmRegs xmm6,xmm7,xmm8,xmm9
        _DeleteFrame rbx,rsi,r12,r13,r14,r15
        ret

Error:  xor eax,eax                             ;set error return code
        jmp Done
Cc4_    endp
        end
