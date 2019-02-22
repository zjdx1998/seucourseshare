        .model flat,c
        .code

; extern "C" void AvxPermuteInt32_(YmmVal* des, YmmVal* src, YmmVal* ind);
;
; Description:  The following function demonstrates use of the
;               vpermd instruction.
;
; Requires:     AVX2

AvxPermuteInt32_ proc
        push ebp
        mov ebp,esp

; Load argument values
        mov eax,[ebp+8]                     ;eax = ptr to des
        mov ecx,[ebp+12]                    ;ecx = ptr to src
        mov edx,[ebp+16]                    ;edx = ptr to ind

; Perform dword permutation
        vmovdqa ymm1,ymmword ptr [edx]      ;ymm1 = ind
        vpermd ymm0,ymm1,ymmword ptr [ecx]
        vmovdqa ymmword ptr [eax],ymm0      ;save result

        vzeroupper
        pop ebp
        ret
AvxPermuteInt32_ endp

; extern "C" void AvxPermuteFloat_(YmmVal* des, YmmVal* src, YmmVal* ind);
;
; Description:  The following function demonstrates use of the 
;               vpermps instruction.
;
; Requires:     AVX2

AvxPermuteFloat_ proc
        push ebp
        mov ebp,esp

; Load argument values
        mov eax,[ebp+8]                     ;eax = ptr to des
        mov ecx,[ebp+12]                    ;ecx = ptr to src
        mov edx,[ebp+16]                    ;edx = ptr to ind

; Perform SPFP permutation
        vmovdqa ymm1,ymmword ptr [edx]      ;ymm1 = ind
        vpermps ymm0,ymm1,ymmword ptr [ecx]
        vmovaps ymmword ptr [eax],ymm0      ;save result

        vzeroupper
        pop ebp
        ret
AvxPermuteFloat_ endp

; extern "C" void AvxPermuteFloatIl_(YmmVal* des, YmmVal* src, YmmVal* ind);
;
; Description:  The following function demonstrates use of the 
;               vpermilps instruction.
;
; Requires:     AVX2

AvxPermuteFloatIl_ proc
        push ebp
        mov ebp,esp

; Load argument values
        mov eax,[ebp+8]                     ;eax = ptr to des
        mov ecx,[ebp+12]                    ;ecx = ptr to src
        mov edx,[ebp+16]                    ;edx = ptr to ind

; Perform in-lane SPFP permutation.  Note that the second source
; operand of vpermilps specifies the indices.
        vmovdqa ymm1,ymmword ptr [ecx]      ;ymm1 = src
        vpermilps ymm0,ymm1,ymmword ptr [edx]
        vmovaps ymmword ptr [eax],ymm0      ;save result

        vzeroupper
        pop ebp
        ret
AvxPermuteFloatIl_ endp
        end
