        .model flat,c
        .code

; extern "C" void SsePfpConvert_(const XmmVal* a, XmmVal* b, CvtOp cvt_op);
;
; Description:  The following function demonstrates use of the packed
;               floating-point conversion instructions.
;
; Requires:     SSE2

SsePfpConvert_ proc
        push ebp
        mov ebp,esp

; Load arguments and make sure 'cvt_op' is valid
        mov eax,[ebp+8]                     ;eax = 'a'
        mov ecx,[ebp+12]                    ;ecx = 'b'
        mov edx,[ebp+16]                    ;edx =cvt_op
        cmp edx,CvtOpTableCount
        jae BadCvtOp
        jmp [CvtOpTable+edx*4]              ;jump to specified conversion

; Convert packed doubleword signed integers to packed SPFP values
SseCvtdq2ps:
        movdqa xmm0,[eax]
        cvtdq2ps xmm1,xmm0
        movaps [ecx],xmm1
        pop ebp
        ret

; Convert packed doubleword signed integers to packed DPFP values
SseCvtdq2pd:
        movdqa xmm0,[eax]
        cvtdq2pd xmm1,xmm0
        movapd [ecx],xmm1
        pop ebp
        ret

; Convert packed SPFP values to packed doubleword signed integers
SseCvtps2dq:
        movaps xmm0,[eax]
        cvtps2dq xmm1,xmm0
        movdqa [ecx],xmm1
        pop ebp
        ret

; Convert packed DPFP values to packed doubleword signed integers
SseCvtpd2dq:
        movapd xmm0,[eax]
        cvtpd2dq xmm1,xmm0
        movdqa [ecx],xmm1
        pop ebp
        ret

; Convert packed SPFP to packed DPFP
SseCvtps2pd:
        movaps xmm0,[eax]
        cvtps2pd xmm1,xmm0
        movapd [ecx],xmm1
        pop ebp
        ret

; Convert packed DPFP to packed SPFP
SseCvtpd2ps:
        movapd xmm0,[eax]
        cvtpd2ps xmm1,xmm0
        movaps [ecx],xmm1
        pop ebp
        ret

BadCvtOp:
        pop ebp
        ret

; The order of values in the following table must match the enum CvtOp
; that's defined in SsePackedFloatingPointConversions.cpp.
            align 4
CvtOpTable  dword SseCvtdq2ps, SseCvtdq2pd, SseCvtps2dq
            dword SseCvtpd2dq, SseCvtps2pd, SseCvtpd2ps
CvtOpTableCount equ ($ - CvtOpTable) / size dword
SsePfpConvert_ endp
        end
