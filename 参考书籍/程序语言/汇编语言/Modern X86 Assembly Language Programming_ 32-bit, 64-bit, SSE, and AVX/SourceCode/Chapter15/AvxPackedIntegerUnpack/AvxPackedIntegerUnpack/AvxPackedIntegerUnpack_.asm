        .model flat,c
        .code

; extern "C" void AvxPiUnpackDQ_(YmmVal* a, YmmVal* b, YmmVal c[2]);
;
; Description:  The following function demonstrates use of the
;               vpunpckldq and vpunpckhdq instructions using
;               256-bit wide operands.
;
; Requires:     AVX2

AvxPiUnpackDQ_ proc
        push ebp
        mov ebp,esp

; Load argument values
        mov eax,[ebp+8]                     ;eax = ptr to a
        mov ecx,[ebp+12]                    ;ecx = ptr to b
        mov edx,[ebp+16]                    ;edx = ptr to c
        vmovdqa ymm0,ymmword ptr [eax]      ;ymm0 = a
        vmovdqa ymm1,ymmword ptr [ecx]      ;ymm1 = b

; Perform dword to qword unpacks
        vpunpckldq ymm2,ymm0,ymm1           ;unpack low doublewords
        vpunpckhdq ymm3,ymm0,ymm1           ;unpack high doublewords
        vmovdqa ymmword ptr [edx],ymm2      ;save low result
        vmovdqa ymmword ptr [edx+32],ymm3   ;save high result

        vzeroupper
        pop ebp
        ret
AvxPiUnpackDQ_ endp

; extern "C" void AviPiPackDW_(YmmVal* a, YmmVal* b, YmmVal* c);
;
; Description:  The following function demonstrates use of the
;               vpackssdw using 256-bit wide operands.
;
; Requires:     AVX2

AvxPiPackDW_ proc
        push ebp
        mov ebp,esp

; Load argument values
        mov eax,[ebp+8]                     ;eax = ptr to a
        mov ecx,[ebp+12]                    ;ecx = ptr to b
        mov edx,[ebp+16]                    ;edx = ptr to c
        vmovdqa ymm0,ymmword ptr [eax]      ;ymm0 = a
        vmovdqa ymm1,ymmword ptr [ecx]      ;ymm1 = b

; Perform pack dword to word with signed saturation
        vpackssdw ymm2,ymm0,ymm1            ;ymm2 = packed words
        vmovdqa ymmword ptr [edx],ymm2      ;save result

        vzeroupper
        pop ebp
        ret
AvxPiPackDW_ endp
        end
