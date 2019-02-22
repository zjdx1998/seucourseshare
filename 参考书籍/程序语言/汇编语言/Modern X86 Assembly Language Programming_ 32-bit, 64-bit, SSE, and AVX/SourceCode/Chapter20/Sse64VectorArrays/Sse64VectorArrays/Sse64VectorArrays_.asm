        include <MacrosX86-64.inc>
        .code

; This structure must match the VectorSoA structure that's
; defined in Sse64VectorArray.h
VectorSoA struct
X       qword ?     ;pointer to vector X components
Y       qword ?     ;pointer to vector Y components
Z       qword ?     ;pointer to vector Z components
VectorSoA ends

; extern "C" bool Sse64VectorCrossProd1_(Vector* c, const Vector* a, const Vector* b, Uint32 num_vectors);
;
; Description:  The following function computes the cross product of two
;               3D vectors.

Sse64VectorCrossProd1_ proc frame
        _CreateFrame Vcp1_,0, 32, r12, r13
        _SaveXmmRegs xmm6,xmm7
        _EndProlog

; Perform required argument validations
        test r9d,r9d
        jz Error                            ;jump if num_vectors == 0
        test r9d,3
        jnz Error                           ;jump if num_vectors % 4 != 0

        test rcx,0fh
        jnz Error                           ;jump if a is misaligned
        test rdx,0fh
        jnz Error                           ;jump if b is misaligned
        test r8,0fh
        jnz Error                           ;jump if c is misaligned
        xor rax,rax                         ;rax = common array offset

        align 16
; Load the next two vectors from a and b
@@:     movaps xmm0,[rdx+rax]               ;a[i]
        movaps xmm1,[r8+rax]                ;b[i]
        movaps xmm2,xmm0
        movaps xmm3,xmm1
        movaps xmm4,[rdx+rax+16]            ;a[i+1]
        movaps xmm5,[r8+rax+16]             ;b[i+1]
        movaps xmm6,xmm4
        movaps xmm7,xmm5

; Calculate the cross products and save the results (# = don't care)
        shufps xmm0,xmm0,11001001b          ;xmm0 = # | ax | az | ay
        shufps xmm1,xmm1,11010010b          ;xmm1 = # | by | bx | bz
        mulps xmm0,xmm1
        shufps xmm2,xmm2,11010010b          ;xmm2 = # | ay | ax | az
        shufps xmm3,xmm3,11001001b          ;xmm3 = # | bx | bz | by
        mulps xmm2,xmm3
        subps xmm0,xmm2                     ;xmm0 = # | cz | cy | cx
        movaps [rcx+rax],xmm0               ;save c[i]

        shufps xmm4,xmm4,11001001b          ;xmm4 = # | ax | az | ay
        shufps xmm5,xmm5,11010010b          ;xmm5 = # | by | bx | bz
        mulps xmm4,xmm5
        shufps xmm6,xmm6,11010010b          ;xmm6 = # | ay | ax | az
        shufps xmm7,xmm7,11001001b          ;xmm7 = # | bx | bz | by
        mulps xmm6,xmm7
        subps xmm4,xmm6                     ;xmm4 = # | cz | cy | cx
        movaps [rcx+rax+16],xmm4            ;save c[i+1]

        add rax,32                          ;update array offset
        sub r9d,2
        jnz @B                              ;repeat until done
        mov eax,1                           ;set success return code

Done:   _RestoreXmmRegs xmm6,xmm7
        _DeleteFrame r12, r13
        ret

Error:  xor eax,eax                         ;set error return code
        jmp Done
Sse64VectorCrossProd1_ endp

; extern "C" bool Sse64VectorCrossProd2_(VectorSoA* c, const VectorSoA* a, const VectorSoA* b, Uint32 num_vectors);
;
; Description:  The following function computes the cross products
;               of the vectors in two VectorSoA instances.

Sse64VectorCrossProd2_ proc frame
        _CreateFrame Vcp2_,0,32,rbx,rsi,rdi,r12,r13,r14,r15
        _SaveXmmRegs xmm6,xmm7
        _EndProlog

; Make sure num_vectors is valid
        test r9d,r9d
        jz Error                            ;jump if num_vectors == 0
        test r9d,3
        jnz Error                           ;jump if num_vectors % 4 != 0
        shr r9d,2

; Initialize vector component array pointers
        xor rax,rax                         ;misaligned pointer test value

        mov rbx,[rcx+VectorSoA.X]           ;rbx = ptr to c.X
        or rax,rbx
        mov rsi,[rcx+VectorSoA.Y]           ;rsi = ptr to c.Y
        or rax,rsi
        mov rdi,[rcx+VectorSoA.Z]           ;rdi = ptr to c.Z
        or rax,rdi

        mov r10,[rdx+VectorSoA.X]           ;r10 = ptr to a.X
        or rax,r10
        mov r11,[rdx+VectorSoA.Y]           ;r11 = ptr to a.Y
        or rax,r11
        mov r12,[rdx+VectorSoA.Z]           ;r12 = ptr to a.Z
        or rax,r12

        mov r13,[r8+VectorSoA.X]            ;r13 = ptr to b.X
        or rax,r13
        mov r14,[r8+VectorSoA.Y]            ;r14 = ptr to b.Y
        or rax,r14
        mov r15,[r8+VectorSoA.Z]            ;r15 = ptr to b.C
        or rax,r15

        and rax,0fh                         ;is a pointer misaligned?
        jnz Error                           ;jump if yes

        xor rax,rax                         ;rax = common array offset
        align 16

; Load the next block of four vectors
@@:     movaps xmm0,xmmword ptr [r10+rax]   ;xmm0 = a.X components
        movaps xmm1,xmmword ptr [r11+rax]   ;xmm1 = a.Y components
        movaps xmm2,xmmword ptr [r12+rax]   ;xmm2 = a.Z components
        movaps xmm6,xmm1
        movaps xmm7,xmm2
        movaps xmm3,xmmword ptr [r13+rax]   ;xmm3 = b.X components
        movaps xmm4,xmmword ptr [r14+rax]   ;xmm4 = b.Y components
        movaps xmm5,xmmword ptr [r15+rax]   ;xmm5 = b.Z components

; Compute four vector cross products
; c.X[i] = a.Y[i] * b.Z[i] - a.Z[i] * b.Y[i]
; c.Y[i] = a.Z[i] * b.X[i] - a.X[i] * b.Z[i]
; c.Z[i] = a.X[i] * b.Y[i] - a.Y[i] * b.X[i]
        mulps xmm6,xmm5                     ;xmm6 = a.Y * b.Z
        mulps xmm7,xmm4                     ;xmm7 = a.Z * b.Y
        subps xmm6,xmm7                     ;xmm6 = c.X components

        mulps xmm2,xmm3                     ;xmm2 = a.Z * b.X
        mulps xmm5,xmm0                     ;xmm5 = a.X * b.Z
        subps xmm2,xmm5                     ;xmm2 = c.Y components

        mulps xmm0,xmm4                     ;xmm0 = a.X * b.Y
        mulps xmm1,xmm3                     ;xmm1 = a.Y * b.X
        subps xmm0,xmm1                     ;xmm0 = c.Z components

        movaps [rdi+rax],xmm0               ;save c.Z
        movaps [rsi+rax],xmm2               ;save c.Y
        movaps [rbx+rax],xmm6               ;save c.X

        add rax,16                          ;update array offset
        sub r9d,1
        jnz @B                              ;repeat until done
        mov eax,1                           ;set success return code

Done:   _RestoreXmmRegs xmm6,xmm7
        _DeleteFrame rbx,rsi,rdi,r12,r13,r14,r15
        ret

Error:  xor eax,eax                         ;set error return code
        jmp Done
Sse64VectorCrossProd2_ endp
        end
