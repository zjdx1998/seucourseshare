        include <MacrosX86-64.inc>
        extern NUM_PIXELS_MAX:dword

        .const
; All of these values must be aligned to a 16-byte boundary
Uint8ToFloat        real4 255.0, 255.0, 255.0, 255.0
FloatToUint8Min     real4 0.0, 0.0, 0.0, 0.0
FloatToUint8Max     real4 1.0, 1.0, 1.0, 1.0
FloatToUint8Scale   real4 255.0, 255.0, 255.0, 255.0
        .code

; extern "C" bool ImageUint8ToFloat_(float* des, const Uint8* src, Uint32 num_pixels);
;
; Description:  The following function converts the values in a Uint8
;               pixel buffer to normalized [0.0, 1.0] SPFP.
;
; Requires:     X86-64, SSE2

ImageUint8ToFloat_ proc frame
        _CreateFrame U2F_,0,64
        _SaveXmmRegs xmm10,xmm11,xmm12,xmm13
        _EndProlog

; Make sure num_pixels is valid and pixel buffers are properly aligned
        test r8d,r8d
        jz Error                            ;jump if num_pixels
        cmp r8d,[NUM_PIXELS_MAX]
        ja Error                            ;jump if num_pixels too big
        test r8d,1fh
        jnz Error                           ;jump if num_pixels % 32 != 0
        test rcx,0fh
        jnz Error                           ;jump if des not aligned
        test rdx,0fh
        jnz Error                           ;jump if src not aligned

; Initialize processing loop registers
        shr r8d,5                               ;number of pixel blocks
        movaps xmm4,xmmword ptr [Uint8ToFloat]  ;xmm4 = packed 255.0f
        pxor xmm5,xmm5                          ;xmm5 = packed 0
        align 16

; Load the next block of 32 pixels
@@:     movdqa xmm0,xmmword ptr [rdx]               ;xmm0 = pixel block
        movdqa xmm10,xmmword ptr [rdx+16]           ;xmm10 = pixel block

; Promote the pixel values in xmm0 from unsigned bytes to unsigned dwords
        movdqa xmm2,xmm0
        punpcklbw xmm0,xmm5
        punpckhbw xmm2,xmm5                 ;xmm2 & xmm0 = 8 word pixels
        movdqa xmm1,xmm0
        movdqa xmm3,xmm2
        punpcklwd xmm0,xmm5
        punpckhwd xmm1,xmm5
        punpcklwd xmm2,xmm5
        punpckhwd xmm3,xmm5                 ;xmm3:xmm0 = 16 dword pixels

; Promote the pixel values in xmm10 from unsigned bytes to unsigned dwords
        movdqa xmm12,xmm10
        punpcklbw xmm10,xmm5
        punpckhbw xmm12,xmm5                ;xmm12 & xmm10 = 8 word pixels
        movdqa xmm11,xmm10
        movdqa xmm13,xmm12
        punpcklwd xmm10,xmm5
        punpckhwd xmm11,xmm5
        punpcklwd xmm12,xmm5
        punpckhwd xmm13,xmm5                ;xmm13:xmm10 = 16 dword pixels

; Convert pixel values from dwords to SPFP
        cvtdq2ps xmm0,xmm0
        cvtdq2ps xmm1,xmm1
        cvtdq2ps xmm2,xmm2
        cvtdq2ps xmm3,xmm3                  ;xmm3:xmm0 = 16 SPFP pixels
        cvtdq2ps xmm10,xmm10
        cvtdq2ps xmm11,xmm11
        cvtdq2ps xmm12,xmm12
        cvtdq2ps xmm13,xmm13                ;xmm13:xmm10 = 16 SPFP pixels

; Normalize all pixel values to [0.0, 1.0] and save the results
        divps xmm0,xmm4
        movaps xmmword ptr [rcx],xmm0       ;save pixels 0 - 3
        divps xmm1,xmm4
        movaps xmmword ptr [rcx+16],xmm1    ;save pixels 4 - 7
        divps xmm2,xmm4
        movaps xmmword ptr [rcx+32],xmm2    ;save pixels 8 - 11
        divps xmm3,xmm4
        movaps xmmword ptr [rcx+48],xmm3    ;save pixels 12 - 15

        divps xmm10,xmm4
        movaps xmmword ptr [rcx+64],xmm10   ;save pixels 16 - 19
        divps xmm11,xmm4
        movaps xmmword ptr [rcx+80],xmm11   ;save pixels 20 - 23
        divps xmm12,xmm4
        movaps xmmword ptr [rcx+96],xmm12   ;save pixels 24 - 27
        divps xmm13,xmm4
        movaps xmmword ptr [rcx+112],xmm13  ;save pixels 28 - 31

        add rdx,32                          ;update src ptr
        add rcx,128                         ;update des ptr
        sub r8d,1
        jnz @B                              ;repeat until done
        mov eax,1                           ;set success return code

Done:   _RestoreXmmRegs xmm10,xmm11,xmm12,xmm13
        _DeleteFrame
        ret

Error:  xor eax,eax                         ;set error return code
        jmp done
ImageUint8ToFloat_ endp

; extern "C" bool ImageFloatToUint8_(Uint8* des, const float* src, Uint32 num_pixels);
;
; Description:  The following function converts a normalized [0.0, 1.0]
;               SPFP pixel buffer to Uint8
;
; Requires      X86-64, SSE4.1

ImageFloatToUint8_ proc frame
        _CreateFrame F2U_,0,32
        _SaveXmmRegs xmm6,xmm7
        _EndProlog

; Make sure num_pixels is valid and pixel buffers are properly aligned
        test r8d,r8d
        jz Error                            ;jump if num_pixels
        cmp r8d,[NUM_PIXELS_MAX] 
        ja Error                            ;jump if num_pixels too big
        test r8d,1fh
        jnz Error                           ;jump if num_pixels % 32 != 0
        test rcx,0fh
        jnz Error                           ;jump if des not aligned
        test rdx,0fh
        jnz Error                           ;jump if src not aligned

; Load required packed constants into registers
        movaps xmm5,xmmword ptr [FloatToUint8Scale] ;xmm5 = packed 255.0
        movaps xmm6,xmmword ptr [FloatToUint8Min]   ;xmm6 = packed 0.0
        movaps xmm7,xmmword ptr [FloatToUint8Max]   ;xmm7 = packed 1.0

        shr r8d,4                           ;number of pixel blocks
LP1:    mov r9d,4                           ;num pixel quartets per block

; Convert 16 float pixels to Uint8
LP2:    movaps xmm0,xmmword ptr [rdx]       ;xmm0 = pixel quartet
        movaps xmm1,xmm0
        cmpltps xmm1,xmm6                   ;compare pixels to 0.0
        andnps xmm1,xmm0                    ;clip pixels < 0.0 to 0.0
        movaps xmm0,xmm1                    ;save result

        cmpnleps xmm1,xmm7                  ;compare pixels to 1.0
        movaps xmm2,xmm1
        andps xmm1,xmm7                     ;clip pixels > 1.0 to 1.0
        andnps xmm2,xmm0                    ;xmm2 = pixels <= 1.0
        orps xmm2,xmm1                      ;xmm2 = final clipped pixels
        mulps xmm2,xmm5                     ;xmm2 = FP pixels [0.0, 255.0]

        cvtps2dq xmm1,xmm2                  ;xmm1 = dword pixels [0, 255]
        packusdw xmm1,xmm1                  ;xmm1[63:0] = word pixels
        packuswb xmm1,xmm1                  ;xmm1[31:0] = bytes pixels

; Save the current byte pixel quartet
        pextrd eax,xmm1,0                   ;eax = new pixel quartet
        psrldq xmm3,4                       ;adjust xmm3 for new quartet
        pinsrd xmm3,eax,3                   ;xmm3[127:96] = new quartet

        add rdx,16                          ;update src ptr
        sub r9d,1
        jnz LP2                             ;repeat until done

; Save the current byte pixel block (16 pixels)
        movdqa xmmword ptr [rcx],xmm3       ;save current pixel block
        add rcx,16                          ;update des ptr
        sub r8d,1
        jnz LP1                             ;repeat until done
        mov eax,1                           ;set success return code

Done:   _RestoreXmmRegs xmm6,xmm7
        _DeleteFrame
        ret

Error:  mov eax,eax                         ;set error return code
        jmp Done
ImageFloatToUint8_ endp
        end
