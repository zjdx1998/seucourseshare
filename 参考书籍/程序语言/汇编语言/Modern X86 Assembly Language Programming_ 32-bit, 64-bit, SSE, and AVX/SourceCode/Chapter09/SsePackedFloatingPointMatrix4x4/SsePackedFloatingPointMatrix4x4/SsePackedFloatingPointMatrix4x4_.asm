        .model flat,c
        .code

; _Mat4x4Transpose macro
;
; Description:  This macro computes the transpose of a 4x4
;               single-precision floating-point matrix.
;
;   Input Matrix                    Output Matrtix
;   xmm0    a3 a2 a1 a0             xmm4    d0 c0 b0 a0
;   xmm1    b3 b2 b1 b0             xmm5    d1 c1 b1 a1
;   xmm2    c3 c2 c1 c0             xmm6    d2 c2 b2 a2
;   xmm3    d3 d2 d1 d0             xmm7    d3 c3 b3 a3
;
; Note:     The row of a 4x4 matrix is reversed when loaded into an
;           XMM register due to x86 little-endian ordering.
;
; Requires: SSE

_Mat4x4Transpose macro
        movaps xmm4,xmm0
        unpcklps xmm4,xmm1                  ;xmm4 = b1 a1 b0 a0
        unpckhps xmm0,xmm1                  ;xmm0 = b3 a3 b2 a2
        movaps xmm5,xmm2
        unpcklps xmm5,xmm3                  ;xmm5 = d1 c1 d0 c0
        unpckhps xmm2,xmm3                  ;xmm2 = d3 c3 d2 c2

        movaps xmm1,xmm4
        movlhps xmm4,xmm5                   ;xmm4 = d0 c0 b0 a0
        movhlps xmm5,xmm1                   ;xmm5 = d1 c1 b1 a1
        movaps xmm6,xmm0
        movlhps xmm6,xmm2                   ;xmm6 = d2 c2 b2 a2
        movaps xmm7,xmm2
        movhlps xmm7,xmm0                   ;xmm7 = d3 c3 b2 a3
        endm

; extern "C" void SsePfpMatrix4x4Multiply_(Mat4x4 m_des, Mat4x4 m_src1, Mat4x4 m_src2);
;
; Description:  The following function computes the product of two
;               4x4 single-precision floating-point matrices.
;
; Requires: SSE4.1

SsePfpMatrix4x4Multiply_ proc
        push ebp
        mov ebp,esp
        push ebx

; Compute transpose of m_src2 (m_src2_T)
        mov ebx,[ebp+16]                    ;ebx = m_src2
        movaps xmm0,[ebx]
        movaps xmm1,[ebx+16]
        movaps xmm2,[ebx+32]
        movaps xmm3,[ebx+48]                ;xmm3:xmm0 = m_src2
        _Mat4x4Transpose                    ;xmm7:xmm4 = m_src2_T

; Perform initializations for matrix product
        mov edx,[ebp+8]                     ;edx = m_des
        mov ebx,[ebp+12]                    ;ebx = m_src1
        mov ecx,4                           ;ecx = number of rows
        xor eax,eax                         ;eax = offset into arrays

; Repeat loop until matrix product is calculated.
        align 16
@@:     movaps xmm0,[ebx+eax]               ;xmm0 = row i of m_src1

; Compute dot product of m_src1 row i and m_src2_T row 0
        movaps xmm1,xmm0
        dpps xmm1,xmm4,11110001b            ;xmm1[31:0] = dot product
        insertps xmm3,xmm1,00000000b        ;xmm3[31:0] = xmm1[31:0]

; Compute dot product of m_src1 row i and m_src2_T row 1
        movaps xmm2,xmm0
        dpps xmm2,xmm5,11110001b            ;xmm2[31:0] = dot product
        insertps xmm3,xmm2,00010000b        ;xmm3[63:32] = xmm2[31:0]

; Compute dot product of m_src1 row i and m_src2_T row 2
        movaps xmm1,xmm0
        dpps xmm1,xmm6,11110001b            ;xmm1[31:0] = dot product
        insertps xmm3,xmm1,00100000b        ;xmm3[95:64] = xmm1[31:0]

; Compute dot product of m_src1 row i and m_src2_T row 3
        movaps xmm2,xmm0
        dpps xmm2,xmm7,11110001b            ;xmm2[31:0] = dot product
        insertps xmm3,xmm2,00110000b        ;xmm3[127:96] = xmm2[31:0]

; Save m_des.row i and update loop variables
        movaps [edx+eax],xmm3               ;save current row result
        add eax,16                          ;set array offset to next row
        dec ecx
        jnz @B

        pop ebx
        pop ebp
        ret
SsePfpMatrix4x4Multiply_ endp

; extern void SsePfpMatrix4x4TransformVectors_(Vec4x1* v_des, Mat4x4 m_src, Vec4x1* v_src, int num_vec);
;
; Description:  The following function applies a transformation matrix
;               to an array 4x1 single-precision floating-point vectors.
;
; Requires:     SSE4.1

SsePfpMatrix4x4TransformVectors_ proc
        push ebp
        mov ebp,esp
        push esi
        push edi

; Make sure num_vec is valid
        mov ecx,[ebp+20]                    ;ecx = num_vec
        test ecx,ecx
        jle Done                            ;jump if num_vec <= 0

; Load m_src into xmm3:xmm0
        mov eax,[ebp+12]                    ;eax = pointer to m_src
        movaps xmm0,[eax]                   ;xmm0 = row 0
        movaps xmm1,[eax+16]                ;xmm1 = row 1
        movaps xmm2,[eax+32]                ;xmm2 = row 2
        movaps xmm3,[eax+48]                ;xmm3 = row 3

; Initialize pointers to v_src and v_des
        mov esi,[ebp+16]                    ;esi = pointer to v_src
        mov edi,[ebp+8]                     ;edi = pointer to v_des
        xor eax,eax                         ;eax = array offset

; Compute v_des[i] = m_src * v_src[i]
        align 16
@@:     movaps xmm4,[esi+eax]               ;xmm4 = vector v_src[i]

; Compute dot product of m_src row 0 and v_src[i]
        movaps xmm5,xmm4
        dpps xmm5,xmm0,11110001b            ;xmm5[31:0] = dot product
        insertps xmm7,xmm5,00000000b        ;xmm7[31:0] = xmm5[31:0]

; Compute dot product of m_src row 1 and v_src[i]
        movaps xmm6,xmm4
        dpps xmm6,xmm1,11110001b            ;xmm6[31:0] = dot product
        insertps xmm7,xmm6,00010000b        ;xmm7[63:32] = xmm6[31:0]

; Compute dot product of m_src row 2 and v_src[i]
        movaps xmm5,xmm4
        dpps xmm5,xmm2,11110001b            ;xmm5[31:0] = dot product
        insertps xmm7,xmm5,00100000b        ;xmm7[95:64] = xmm5[31:0]

; Compute dot product of m_src row 3 and v_src[i]
        movaps xmm6,xmm4
        dpps xmm6,xmm3,11110001b            ;xmm6[31:0] = dot product
        insertps xmm7,xmm6,00110000b        ;xmm7[127:96] = xmm6[31:0]

; Save v_des[i] and update loop variables
        movaps [edi+eax],xmm7               ;save transformed vector
        add eax,16
        dec ecx
        jnz @B

Done:   pop edi
        pop esi
        pop ebp
        ret
SsePfpMatrix4x4TransformVectors_ endp
        end
