        .model flat,c
        .code

; extern "C" void AvxPiI16_(YmmVal* a, YmmVal* b, YmmVal c[6]);
;
; Description:  The following function illustrates use of various
;               packed 16-bit integer arithmetic instructions
;               using 256-bit wide operands.
;
; Requires:     AVX2

AvxPiI16_ proc
        push ebp
        mov ebp,esp

; Load argument values
        mov eax,[ebp+8]                     ;eax = ptr to a
        mov ecx,[ebp+12]                    ;ecx = ptr to b
        mov edx,[ebp+16]                    ;edx = ptr to c

; Load a and b, which must be properly aligned
        vmovdqa ymm0,ymmword ptr [eax]      ;ymm0 = a
        vmovdqa ymm1,ymmword ptr [ecx]      ;ymm1 = b

; Perform packed arithmetic operations
        vpaddw ymm2,ymm0,ymm1               ;add
        vpaddsw ymm3,ymm0,ymm1              ;add with signed saturation
        vpsubw ymm4,ymm0,ymm1               ;sub
        vpsubsw ymm5,ymm0,ymm1              ;sub with signed saturation
        vpminsw ymm6,ymm0,ymm1              ;signed minimums
        vpmaxsw ymm7,ymm0,ymm1              ;signed maximums

; Save results
        vmovdqa ymmword ptr [edx],ymm2          ;save vpaddw result
        vmovdqa ymmword ptr [edx+32],ymm3       ;save vpaddsw result
        vmovdqa ymmword ptr [edx+64],ymm4       ;save vpsubw result
        vmovdqa ymmword ptr [edx+96],ymm5       ;save vpsubsw result
        vmovdqa ymmword ptr [edx+128],ymm6      ;save vpminsw result
        vmovdqa ymmword ptr [edx+160],ymm7      ;save vpmaxsw result

        vzeroupper
        pop ebp
        ret
AvxPiI16_ endp

; extern "C" void AvxPiI32_(YmmVal* a, YmmVal* b, YmmVal c[5]);
;
; Description:  The following function illustrates use of various
;               packed 32-bit integer arithmetic instructions
;               using 256-bit wide operands.
;
; Requires:     AVX2

AvxPiI32_ proc
        push ebp
        mov ebp,esp

; Load argument values
        mov eax,[ebp+8]                     ;eax = ptr to a
        mov ecx,[ebp+12]                    ;ecx = ptr to b
        mov edx,[ebp+16]                    ;edx = ptr to c

; Load a and b, which must be properly aligned
        vmovdqa ymm0,ymmword ptr [eax]      ;ymm0 = a
        vmovdqa ymm1,ymmword ptr [ecx]      ;ymm1 = b

; Perform packed arithmetic operations
        vphaddd ymm2,ymm0,ymm1              ;horizontal add
        vphsubd ymm3,ymm0,ymm1              ;horizontal sub
        vpmulld ymm4,ymm0,ymm1              ;signed mul (low 32 bits)
        vpsllvd ymm5,ymm0,ymm1              ;shift left logical
        vpsravd ymm6,ymm0,ymm1              ;shift right arithmetic

; Save results
        vmovdqa ymmword ptr [edx],ymm2      ;save vphaddd result
        vmovdqa ymmword ptr [edx+32],ymm3   ;save vphsubd result
        vmovdqa ymmword ptr [edx+64],ymm4   ;save vpmulld result
        vmovdqa ymmword ptr [edx+96],ymm5   ;save vpsllvd result
        vmovdqa ymmword ptr [edx+128],ymm6  ;save vpsravd result

        vzeroupper
        pop ebp
        ret
AvxPiI32_ endp
        end
