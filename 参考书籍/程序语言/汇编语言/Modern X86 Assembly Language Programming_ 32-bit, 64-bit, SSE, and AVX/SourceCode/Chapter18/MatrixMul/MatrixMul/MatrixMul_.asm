        include <MacrosX86-64.inc>
        .code
        extern malloc:proc

; extern "C" double* MatrixMul_(const double* m1, int nr1, int nc1, const double* m2, int nr2, int nc2);
;
; Description:  The following function computes the product of two
;               matrices.

MatrixMul_ proc frame
        _CreateFrame MatMul_,0,0,rbx,r12,r13,r14,r15
        _EndProlog

; Verify the matrix size values.
        movsxd r12,edx                              ;r12 = nr1
        test r12,r12
        jle Error                                   ;jump if nr1 <= 0

        movsxd r13,r8d                              ;r13 = nc1
        test r13,r13
        jle Error                                   ;jump if nc1 <= 0

        movsxd r14,dword ptr [rbp+MatMul_OffsetStackArgs]     ;r14 = nr2
        test r14,r14
        jle Error                                   ;jump if nr2 <= 0

        movsxd r15,dword ptr [rbp+MatMul_OffsetStackArgs+8]   ;r15 = nc2
        test r15,r15
        jle Error                                   ;jump if nc2 <= 0

        cmp r13,r14
        jne Error                                   ;jump if nc1 != nr2

; Allocate storage
        mov [rbp+MatMul_OffsetHomeRCX],rcx  ;save m1
        mov [rbp+MatMul_OffsetHomeR9],r9    ;save m2
        mov rcx,r12                         ;rcx = nr1
        imul rcx,r15                        ;rcx = nr1 * nc2
        shl rcx,3                           ;rcx = nr1 * nc2 * size real8
        sub rsp,32                          ;allocate home space
        call malloc
        mov rbx,rax                         ;rbx = ptr to m3

; Initialize source matrix pointers and row index i
        mov rcx,[rbp+MatMul_OffsetHomeRCX]  ;rcx = ptr to m1
        mov rdx,[rbp+MatMul_OffsetHomeR9]   ;rdx = ptr to m2
        xor r8,r8                           ;i = 0

; Initialize column index j
Lp1:    xor r9,r9                           ;j = 0

; Initialize sum and index k
Lp2:    xorpd xmm4,xmm4                     ;sum = 0;
        xor r10,r10                         ;k = 0;

; Calculate sum += m1[i * nc1 + k] * m2[k * nc2 + j]
Lp3:    mov rax,r8                          ;rax = i
        imul rax,r13                        ;rax = i * nc1
        add rax,r10                         ;rax = i * nc1 + k
        movsd xmm0,real8 ptr [rcx+rax*8]    ;xmm0 = m1[i * nc1 + k]

        mov r11,r10                         ;r11 = k;
        imul r11,r15                        ;r11 = k * nc2
        add r11,r9                          ;r11 = k * nc2 + j
        movsd xmm1,real8 ptr [rdx+r11*8]    ;xmm1 = m2[k * nc2 + j]

        mulsd xmm0,xmm1         ;xmm0 = m1[i * nc1 + k] * m2[k * nc2 + j]
        addsd xmm4,xmm0         ;update sum

        inc r10                             ;k++
        cmp r10,r13
        jl Lp3                              ;jump if k < nc1

; Save sum to m3[i * nc2 + j]
        mov rax,r8                          ;rax = i
        imul rax,r15                        ;rax = i * nc2
        add rax,r9                          ;rax = i * nc2 + j
        movsd real8 ptr [rbx+rax*8],xmm4    ;m3[i * nc2 + j] = sum

; Update loop counters and repeat until done
        inc r9                              ;j++
        cmp r9,r15
        jl Lp2                              ;jump if j < nc2
        inc r8                              ;i++
        cmp r8,r12
        jl Lp1                              ;jump if i < nr1

        mov rax,rbx                         ;rax = ptr to m3

Done:   _DeleteFrame rbx,r12,r13,r14,r15
        ret

Error:  xor rax,rax                         ;return NULL
        jmp Done
MatrixMul_ endp
        end
