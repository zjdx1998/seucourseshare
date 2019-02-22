        .model flat,c
        .code

; extern "C" void AvxPfpCompare_(const YmmVal* a, const YmmVal* b, YmmVal c[8]);
;
; Description:  The following function demonstrates use of the
;               x86-AVX compare instruction vcmppd.
;
; Requires:     AVX

AvxPfpCompare_ proc
        push ebp
        mov ebp,esp

; Load argument values
        mov eax,[ebp+8]                     ;eax = ptr to a
        mov ecx,[ebp+12]                    ;ecx = ptr to b
        mov edx,[ebp+16]                    ;edx = ptr to c
        vmovapd ymm0,ymmword ptr [eax]      ;ymm0 = a
        vmovapd ymm1,ymmword ptr [ecx]      ;ymm1 = b

; Compare for equality
        vcmpeqpd ymm2,ymm0,ymm1
        vmovapd ymmword ptr [edx],ymm2

; Compare for inequality
        vcmpneqpd ymm2,ymm0,ymm1
        vmovapd ymmword ptr [edx+32],ymm2

; Compare for less than
        vcmpltpd ymm2,ymm0,ymm1
        vmovapd ymmword ptr [edx+64],ymm2

; Compare for less than or equal
        vcmplepd ymm2,ymm0,ymm1
        vmovapd ymmword ptr [edx+96],ymm2

; Compare for greater than
        vcmpgtpd ymm2,ymm0,ymm1
        vmovapd ymmword ptr [edx+128],ymm2

; Compare for greater than or equal
        vcmpgepd ymm2,ymm0,ymm1
        vmovapd ymmword ptr [edx+160],ymm2

; Compare for ordered
        vcmpordpd ymm2,ymm0,ymm1
        vmovapd ymmword ptr [edx+192],ymm2

; Compare for unordered
        vcmpunordpd ymm2,ymm0,ymm1
        vmovapd ymmword ptr [edx+224],ymm2

; Zero upper 128-bit of all YMM registers to avoid potential x86-AVX
; to x86-SSE transition penalties.
        vzeroupper
        pop ebp
        ret
AvxPfpCompare_ endp
        end
