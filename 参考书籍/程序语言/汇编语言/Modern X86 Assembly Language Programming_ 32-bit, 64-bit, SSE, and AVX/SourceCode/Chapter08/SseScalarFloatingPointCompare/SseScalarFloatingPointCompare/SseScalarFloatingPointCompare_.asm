        .model flat,c
        .code

; extern "C" void SseSfpCompareFloat_(float a, float b, bool* results);
;
; Description:  The following function demonstrates use of the comiss
;               instruction.
;
; Requires      SSE

SseSfpCompareFloat_ proc
        push ebp
        mov ebp,esp

; Load argument values
        movss xmm0,real4 ptr [ebp+8]        ;xmm0 = a
        movss xmm1,real4 ptr [ebp+12]       ;xmm1 = b
        mov edx,[ebp+16]                    ;edx = results array

; Set result flags based on compare status
        comiss xmm0,xmm1
        setp byte ptr [edx]                 ;EFLAGS.PF = 1 if unordered
        jnp @F
        xor al,al
        mov byte ptr [edx+1],al             ;Use default result values
        mov byte ptr [edx+2],al
        mov byte ptr [edx+3],al
        mov byte ptr [edx+4],al
        mov byte ptr [edx+5],al
        mov byte ptr [edx+6],al
        jmp Done

@@:     setb byte ptr [edx+1]               ;set byte if a < b
        setbe byte ptr [edx+2]              ;set byte if a <= b
        sete byte ptr [edx+3]               ;set byte if a == b
        setne byte ptr [edx+4]              ;set byte if a != b
        seta byte ptr [edx+5]               ;set byte if a > b
        setae byte ptr [edx+6]              ;set byte if a >= b

Done:   pop ebp
        ret
SseSfpCompareFloat_ endp

; extern "C" void SseSfpCompareDouble_(double a, double b, bool* results);
;
; Description:  The following function demonstrates use of the comisd
;               instruction.
;
; Requires      SSE2

SseSfpCompareDouble_ proc
        push ebp
        mov ebp,esp

; Load argument values
        movsd xmm0,real8 ptr [ebp+8]        ;xmm0 = a
        movsd xmm1,real8 ptr [ebp+16]       ;xmm1 = b
        mov edx,[ebp+24]                    ;edx = results array

; Set result flags based on compare status
        comisd xmm0,xmm1
        setp byte ptr [edx]                 ;EFLAGS.PF = 1 if unordered
        jnp @F
        xor al,al
        mov byte ptr [edx+1],al             ;Use default result values
        mov byte ptr [edx+2],al
        mov byte ptr [edx+3],al
        mov byte ptr [edx+4],al
        mov byte ptr [edx+5],al
        mov byte ptr [edx+6],al
        jmp Done

@@:     setb byte ptr [edx+1]               ;set byte if a < b
        setbe byte ptr [edx+2]              ;set byte if a <= b
        sete byte ptr [edx+3]               ;set byte if a == b
        setne byte ptr [edx+4]              ;set byte if a != b
        seta byte ptr [edx+5]               ;set byte if a > b
        setae byte ptr [edx+6]              ;set byte if a >= b

Done:   pop ebp
        ret
SseSfpCompareDouble_ endp
        end
