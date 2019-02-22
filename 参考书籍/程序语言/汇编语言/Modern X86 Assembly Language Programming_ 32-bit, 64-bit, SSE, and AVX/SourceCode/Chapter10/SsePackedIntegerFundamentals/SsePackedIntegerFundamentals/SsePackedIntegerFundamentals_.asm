        .model flat,c
        .code

; extern "C" void SsePiAddI16_(const XmmVal* a, const XmmVal* b, XmmVal c[2]);
;
; Description:  The following function demonstrates packed signed word
;               addition using wraparound and saturated modes.
;
; Requires:     SSE2

SsePiAddI16_ proc
        push ebp
        mov ebp,esp

; Initialize
        mov eax,[ebp+8]                     ;eax = pointer to a
        mov ecx,[ebp+12]                    ;ecx = pointer to b
        mov edx,[ebp+16]                    ;edx = pointer to c

; Load XmmVals a and b
        movdqa xmm0,[eax]                   ;xmm0 = a
        movdqa xmm1,xmm0
        movdqa xmm2,[ecx]                   ;xmm2 = b

; Perform packed word additions
        paddw xmm0,xmm2                     ;packed add - wraparound
        paddsw xmm1,xmm2                    ;packed add - saturated

; Save results
        movdqa [edx],xmm0                   ;save c[0]
        movdqa [edx+16],xmm1                ;save c[1]

        pop ebp
        ret
SsePiAddI16_ endp

; extern "C" void SsePiSubI32_(const XmmVal* a, const XmmVal* b, XmmVal* c);
;
; Description:  The following function demonstrates packed signed
;               doubleword subtraction.
;
; Requires:     SSE2

SsePiSubI32_ proc
        push ebp
        mov ebp,esp

; Initialize
        mov eax,[ebp+8]                     ;eax = pointer to a
        mov ecx,[ebp+12]                    ;ecx = pointer to b
        mov edx,[ebp+16]                    ;edx = pointer to c

; Perform packed doubleword subtraction
        movdqa xmm0,[eax]                   ;xmm0 = a
        psubd xmm0,[ecx]                    ;xmm0 = a - b
        movdqu [edx],xmm0                   ;save result to unaligned mem

        pop ebp
        ret
SsePiSubI32_ endp

; extern "C" void SsePiMul32_(const XmmVal* a, const XmmVal* b, XmmVal c[2]);
;
; Description:  The following function demonstrates packed doubleword
;               multiplication.
;
; Requires:     SSE4.1

SsePiMul32_ proc
        push ebp
        mov ebp,esp

; Initialize
        mov eax,[ebp+8]                     ;eax = pointer to a
        mov ecx,[ebp+12]                    ;ecx = pointer to b
        mov edx,[ebp+16]                    ;edx = pointer to c

; Load values and perform the multiplication
        movdqa xmm0,[eax]                   ;xmm0 = a
        movdqa xmm1,[ecx]                   ;xmm1 = b

        movdqa xmm2,xmm0
        pmulld xmm0,xmm1                ;signed dword mul - low result
        pmuldq xmm2,xmm1                ;signed dword mul - qword result

        movdqa [edx],xmm0               ;c[0] = pmulld result
        movdqa [edx+16],xmm2            ;c[1] = pmuldq result

        pop ebp
        ret
SsePiMul32_ endp
        end
