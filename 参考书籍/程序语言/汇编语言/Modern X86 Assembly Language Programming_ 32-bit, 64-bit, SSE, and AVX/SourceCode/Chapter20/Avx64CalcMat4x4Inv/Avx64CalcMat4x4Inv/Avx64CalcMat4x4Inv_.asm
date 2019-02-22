        include <MacrosX86-64.inc>

ConstVals segment readonly align(32) 'const'
VpermpsTranspose    dword 0,4,1,5,2,6,3,7
VpermsTrace         dword 0,2,5,7,0,0,0,0

Mat4x4I         real4 1.0, 0.0, 0.0, 0.0
                real4 0.0, 1.0, 0.0, 0.0
                real4 0.0, 0.0, 1.0, 0.0
                real4 0.0, 0.0, 0.0, 1.0

r4_SignBitMask  dword 80000000h,80000000h,80000000h,80000000h
r4_AbsMask      dword 7fffffffh,7fffffffh,7fffffffh,7fffffffh

r4_1p0          real4 1.0
r4_N1p0         real4 -1.0
r4_N0p5         real4 -0.5
r4_N0p3333      real4 -0.3333333333
r4_N0p25        real4 -0.25
ConstVals ends
        .code

; _Mat4x4TraceYmm macro
;
; Description:  The following macro generates code that calculates the
;               trace of a 4x4 SPFP matrix in registers ymm1:ymm0.

_Mat4x4TraceYmm macro
        vblendps ymm0,ymm0,ymm1,84h             ;copy diagonals to ymm0
        vmovdqa ymm2,ymmword ptr [VpermsTrace]
        vpermps ymm1,ymm2,ymm0                  ;ymm1[127:0] = diagonals
        vhaddps ymm0,ymm1,ymm1
        vhaddps ymm0,ymm0,ymm0                  ;ymm0[31:0] = trace
        endm

; Mat4x4Mul
;
; Description:  The following function computes the product of two
;               4x4 matrices.
;
; Input:        ymm1:ymm0   m1
;               ymm3:ymm2   m2
;
; Output:       ymm1:ymm0   m1 * m2
;
; Notes:        In comments below, m2T denotes the transpose of matrix m2.

Mat4x4Mul proc private

; Calculate transpose of m2
        vmovdqa ymm6,ymmword ptr [VpermpsTranspose] ;ymm6 = vperms indices
        vunpcklps ymm4,ymm2,ymm3
        vunpckhps ymm5,ymm2,ymm3            ;ymm5:ymm4 = partial transpose
        vpermps ymm2,ymm6,ymm4
        vpermps ymm3,ymm6,ymm5              ;ymm3:ymm2 = m2T

; Copy rows of m2T to ymm*[255:128] and ymm*[127:0]
        vperm2f128 ymm4,ymm2,ymm2,00000000b     ;ymm4 = m2T.row0
        vperm2f128 ymm5,ymm2,ymm2,00010001b     ;ymm5 = m2T.row1
        vperm2f128 ymm6,ymm3,ymm3,00000000b     ;ymm6 = m2T.row2
        vperm2f128 ymm7,ymm3,ymm3,00010001b     ;ymm7 = m2T.row3

; Perform mat4x4 multiplication, rows 0 and 1
; Note that all unused vdpps destination register elements are set to zero
; ymm8[31:0]      = dp(m1.row0, m2T.row0)
; ymm8[159:128]   = dp(m1.row1, m2T.row0)
; ymm9[63:32]     = dp(m1.row0, m2T.row1)
; ymm9[191:160]   = dp(m1.row1, m2T.row1)
; ymm10[95:64]    = dp(m1.row0, m2T.row2)
; ymm10[223:192]  = dp(m1.row1, m2T.row2)
; ymm11[127:96]   = dp(m1.row0, m2T.row3)
; ymm11[255:224]  = dp(m1.row1, m2T.row3)
        vdpps ymm8,ymm0,ymm4,11110001b
        vdpps ymm9,ymm0,ymm5,11110010b
        vdpps ymm10,ymm0,ymm6,11110100b
        vdpps ymm11,ymm0,ymm7,11111000b
        vorps ymm8,ymm8,ymm9
        vorps ymm10,ymm10,ymm11
        vorps ymm0,ymm8,ymm10               ;ymm0 = rows 0 and 1

; Perform mat4x4 multiplication, rows 2 and 3
; ymm8[31:0]      = dp(m1.row2, m2T.row0)
; ymm8[159:128]   = dp(m1.row3, m2T.row0)
; ymm9[63:32]     = dp(m1.row2, m2T.row1)
; ymm9[191:160]   = dp(m1.row3, m2T.row1)
; ymm10[95:64]    = dp(m1.row2, m2T.row2)
; ymm10[223:192]  = dp(m1.row3, m2T.row2)
; ymm11[127:96]   = dp(m1.row2, m2T.row3)
; ymm11[255:224]  = dp(m1.row3, m2T.row3)
        vdpps ymm8,ymm1,ymm4,11110001b
        vdpps ymm9,ymm1,ymm5,11110010b
        vdpps ymm10,ymm1,ymm6,11110100b
        vdpps ymm11,ymm1,ymm7,11111000b
        vorps ymm8,ymm8,ymm9
        vorps ymm10,ymm10,ymm11
        vorps ymm1,ymm8,ymm10               ;ymm1 = rows 2 and 3
        ret
Mat4x4Mul endp

; extern "C" bool Mat4x4Inv_(Mat4x4 m_inv, Mat4x4 m, float epsilon, bool* is_singular);
;
; Description:  The following function computes the inverse of a 4x4
;               matrix.
;
; Requires:     x86-64, AVX2
;
; Notes:        In the comments below, m2 = m * m, m3 = m * m * m, etc.

; Offsets of temporary values on the stack
OffsetM2Lo equ 0                            ;m2 rows 0 and 1
OffsetM2Hi equ 32                           ;m2 rows 2 and 3
OffsetM3Lo equ 64                           ;m3 rows 0 and 1
OffsetM3Hi equ 96                           ;m3 rows 2 and 3

Mat4x4Inv_ proc frame
        _CreateFrame Minv_,16,160
        _SaveXmmRegs xmm6,xmm7,xmm8,xmm9,xmm10,xmm11,xmm12,xmm13,xmm14,xmm15
        _EndProlog

; Perform required initializations and validations
        test rcx,01fh
        jnz Error                           ;jump if m_inv is misaligned
        test rdx,01fh
        jnz Error                           ;jump if m is misaligned
        vmovaps ymm14,[rdx]
        vmovaps ymm15,[rdx+32]              ;ymm15:ymm14 = m
        vmovss real4 ptr [rbp],xmm2         ;save epsilon for later use

; Allocate 128 bytes of 32-byte aligned stack space for temp matrices
        and rsp,0ffffffe0h                  ;align rsp to 32-byte boundary
        sub rsp,128                         ;alloc space for temp matrices

; Compute m2
        vmovaps ymm0,ymm14
        vmovaps ymm1,ymm15                  ;ymm1:ymm0 = m
        vmovaps ymm2,ymm14
        vmovaps ymm3,ymm15                  ;ymm3:ymm2 = m
        call Mat4x4Mul                      ;ymm1:ymm0 = m2
        vmovaps [rsp+OffsetM2Lo],ymm0
        vmovaps [rsp+OffsetM2Hi],ymm1       ;save m2

; Compute m3
        vmovaps ymm2,ymm14
        vmovaps ymm3,ymm15                  ;ymm3:ymm2 = m
        call Mat4x4Mul                      ;ymm1:ymm0 = m3
        vmovaps [rsp+OffsetM3Lo],ymm0
        vmovaps [rsp+OffsetM3Hi],ymm1       ;save m3
        vmovaps ymm12,ymm0
        vmovaps ymm13,ymm1                  ;ymm13:ymm12 = m3

; Compute m4
        vmovaps ymm2,ymm14
        vmovaps ymm3,ymm15                  ;ymm3:ymm2 = m
        call Mat4x4Mul                      ;ymm1:ymm0 = m4

; Compute and save matrix trace values
        _Mat4x4TraceYmm
        vmovss xmm10,xmm0,xmm0              ;xmm10 = t4

        vmovaps ymm0,ymm12
        vmovaps ymm1,ymm13
        _Mat4x4TraceYmm
        vmovss xmm9,xmm0,xmm0               ;xmm9 = t3

        vmovaps ymm0,[rsp+OffsetM2Lo]
        vmovaps ymm1,[rsp+OffsetM2Hi]
        _Mat4x4TraceYmm
        vmovss xmm8,xmm0,xmm0               ;xmm8 = t2

        vmovaps ymm0,ymm14
        vmovaps ymm1,ymm15
        _Mat4x4TraceYmm
        vmovss xmm7,xmm0,xmm0               ;xmm7 = t1

; Calculate the required coefficients
; c1 = -t1;
; c2 = -1.0f / 2.0f * (c1 * t1 + t2);
; c3 = -1.0f / 3.0f * (c2 * t1 + c1 * t2 + t3);
; c4 = -1.0f / 4.0f * (c3 * t1 + c2 * t2 + c1 * t3 + t4);
;
; Registers used: t1-t4 = xmm7-xmm10, c1-c4 = xmm12-xmm15
        vxorps xmm12,xmm7,real4 ptr [r4_SignBitMask]    ;xmm12 = c1

        vmulss xmm13,xmm12,xmm7         ;c1 * t1
        vaddss xmm13,xmm13,xmm8         ;c1 * t1 + t2
        vmulss xmm13,xmm13,[r4_N0p5]    ;xmm13 = c2

        vmulss xmm14,xmm13,xmm7         ;c2 * t1
        vmulss xmm0,xmm12,xmm8          ;c1 * t2
        vaddss xmm14,xmm14,xmm0         ;c2 * t1 + c1 * t2
        vaddss xmm14,xmm14,xmm9         ;c2 * t1 + c1 * t2 + t3
        vmulss xmm14,xmm14,[r4_N0p3333] ;xmm14 = c3

        vmulss xmm15,xmm14,xmm7         ;c3 * t1
        vmulss xmm0,xmm13,xmm8          ;c2 * t2
        vmulss xmm1,xmm12,xmm9          ;c1 * t3
        vaddss xmm2,xmm0,xmm1           ;c2 * t2 + c1 * t3
        vaddss xmm15,xmm15,xmm2         ;c3 * t1 + c2 * t2 + c1 * t3
        vaddss xmm15,xmm15,xmm10        ;c3 * t1 + c2 * t2 + c1 * t3 + t4
        vmulss xmm15,xmm15,[r4_N0p25]   ;xmm15 = c4

; Make sure matrix is not singular
        vandps xmm1,xmm15,[r4_AbsMask]  ;compute fabs(c4)
        vcomiss xmm1,real4 ptr [rbp]    ;compare against epsilon
        setp al                         ;set al = if unordered
        setb ah                         ;set ah = if fabs(c4) < epsilon
        or al,ah                        ;al = is_singular
        mov [r9],al                     ;save is_singular state
        jnz Error                       ;jump if singular

; Calculate m_inv = -1.0 / c4 * (m3 + c1 * m2 + c2 * m1 + c3 * I)
        vmovaps ymm0,[rsp+OffsetM3Lo]
        vmovaps ymm1,[rsp+OffsetM3Hi]       ;ymm1:ymm0 = m3

        vbroadcastss ymm12,xmm12
        vmulps ymm2,ymm12,[rsp+OffsetM2Lo]
        vmulps ymm3,ymm12,[rsp+OffsetM2HI]  ;ymm3:ymm2 = c1 * m2

        vbroadcastss ymm13,xmm13
        vmulps ymm4,ymm13,[rdx]
        vmulps ymm5,ymm13,[rdx+32]          ;ymm5:ymm4 = c2 * m

        vbroadcastss ymm14,xmm14
        vmulps ymm6,ymm14,[Mat4x4I]
        vmulps ymm7,ymm14,[Mat4x4I+32]      ;ymm7:ymm6 = c3 * I

        vaddps ymm0,ymm0,ymm2
        vaddps ymm1,ymm1,ymm3               ;ymm1:ymm0 = m3 + c1*m2
        vaddps ymm8,ymm4,ymm6
        vaddps ymm9,ymm5,ymm7               ;ymm9:ymm8 = c2*m + c3*I
        vaddps ymm0,ymm0,ymm8
        vaddps ymm1,ymm1,ymm9               ;ymm1:ymm0 = matrix sum

        vmovss xmm2,[r4_N1p0]
        vdivss xmm2,xmm2,xmm15              ;xmm2 = -1.0 / c4
        vbroadcastss ymm2,xmm2
        vmulps ymm0,ymm0,ymm2
        vmulps ymm1,ymm1,ymm2               ;ymm1:ymm0 = m_inv

        vmovaps [rcx],ymm0
        vmovaps [rcx+32],ymm1               ;save m_inv
        mov eax,1                           ;set success return code

Done:    vzeroupper
        _RestoreXmmRegs xmm6,xmm7,xmm8,xmm9,xmm10,xmm11,xmm12,xmm13,xmm14,xmm15
        _DeleteFrame
        ret

Error:  xor eax,eax
        jmp Done
Mat4x4Inv_ endp

; The following functions are for software test & debug.
Mat4x4Trace_ proc
        _Mat4x4TraceYmm
        ret
Mat4x4Trace_ endp
Mat4x4Mul_ proc
        call Mat4x4Mul
        ret
Mat4x4Mul_ endp
        end
