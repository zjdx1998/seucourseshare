        .model flat,c
        .code

; extern "C" void AvxBlendFloat_(YmmVal* des, YmmVal* src1, YmmVal* src2, YmmVal* src3);
;
; Description:  The following function demonstrates used of the vblendvps
;               instruction using YMM registers.
;
; Requires:     AVX

AvxBlendFloat_ proc
        push ebp
        mov ebp,esp

; Load argument values
        mov eax,[ebp+12]                    ;eax = ptr to src1
        mov ecx,[ebp+16]                    ;ecx = ptr to src2
        mov edx,[ebp+20]                    ;edx = ptr to src3

        vmovaps ymm1,ymmword ptr [eax]      ;ymm1 = src1
        vmovaps ymm2,ymmword ptr [ecx]      ;ymm2 = src2
        vmovdqa ymm3,ymmword ptr [edx]      ;ymm3 = src3

; Perform variable SPFP blend
        vblendvps ymm0,ymm1,ymm2,ymm3       ;ymm0 = blend result
        mov eax,[ebp+8]
        vmovaps ymmword ptr [eax],ymm0      ;save blend result

        vzeroupper
        pop ebp
        ret
AvxBlendFloat_ endp

; extern "C" void AvxBlendByte_(YmmVal* des, YmmVal* src1, YmmVal* src2, YmmVal* src3);
;
; Description:  The following function demonstrates use of the vpblendvb
;               instruction.
;
; Requires:     AVX2

AvxBlendByte_ proc
        push ebp
        mov ebp,esp

; Load argument values
        mov eax,[ebp+12]                    ;eax = ptr to src1
        mov ecx,[ebp+16]                    ;ecx = ptr to src2
        mov edx,[ebp+20]                    ;edx = ptr to src3

        vmovdqa ymm1,ymmword ptr [eax]      ;ymm1 = src1
        vmovdqa ymm2,ymmword ptr [ecx]      ;ymm2 = src2
        vmovdqa ymm3,ymmword ptr [edx]      ;ymm3 = src3

; Perform variable byte blend
        vpblendvb ymm0,ymm1,ymm2,ymm3       ;ymm0 = blend result
        mov eax,[ebp+8]
        vmovdqa ymmword ptr [eax],ymm0      ;save blend result
        vzeroupper
        pop ebp
        ret
AvxBlendByte_ endp
        end
