        .model flat,c
        .code

; extern "C" void AvxGatherFloat_(YmmVal* des, YmmVal* indices, YmmVal* mask, const float* x);
;
; Description:  The following function demonstrates use of the
;               vgatherdps instruction.
;
; Requires:     AVX2

AvxGatherFloat_ proc
        push ebp
        mov ebp,esp
        push ebx

; Load argument values. The contents of des are loaded into ymm0
; prior to execution of the vgatherdps instruction in order to
; demonstrate the conditional effects of the control mask.
        mov eax,[ebp+8]                     ;eax = ptr to des
        mov ebx,[ebp+12]                    ;ebx = ptr to indices
        mov ecx,[ebp+16]                    ;ecx = ptr to mask
        mov edx,[ebp+20]                    ;edx = ptr to x
        vmovaps ymm0,ymmword ptr [eax]      ;ymm0 = des (initial values)
        vmovdqa ymm1,ymmword ptr [ebx]      ;ymm1 = indices
        vmovdqa ymm2,ymmword ptr [ecx]      ;ymm2 = mask

; Perform the gather operation and save the results.
        vgatherdps ymm0,[edx+ymm1*4],ymm2   ;ymm0 = gathered elements
        vmovaps ymmword ptr [eax],ymm0      ;save des
        vmovdqa ymmword ptr [ebx],ymm1      ;save indices (unchanged)
        vmovdqa ymmword ptr [ecx],ymm2      ;save mask (all zeros)

        vzeroupper
        pop ebx
        pop ebp
        ret
AvxGatherFloat_ endp

; extern "C" void AvxGatherI64_(YmmVal* des, XmmVal* indices, YmmVal* mask, const Int64* x);
;
; Description:  The following function demonstrates use of the vpgatherdq
;               instruction.
;
; Requires:     AVX2

AvxGatherI64_ proc
        push ebp
        mov ebp,esp
        push ebx

; Load argument values. Note that the indices are loaded
; into register XMM1.
        mov eax,[ebp+8]                     ;eax = ptr to des
        mov ebx,[ebp+12]                    ;ebx = ptr to indices
        mov ecx,[ebp+16]                    ;ecx = ptr to mask
        mov edx,[ebp+20]                    ;edx = ptr to x
        vmovdqa ymm0,ymmword ptr [eax]      ;ymm0 = des (initial values)
        vmovdqa xmm1,xmmword ptr [ebx]      ;xmm1 = indices
        vmovdqa ymm2,ymmword ptr [ecx]      ;ymm2 = mask

; Perform the gather and save the results.  Note that the
; scale factor matches the size of the gathered elements.
        vpgatherdq ymm0,[edx+xmm1*8],ymm2   ;ymm0 = gathered elements
        vmovdqa ymmword ptr [eax],ymm0      ;save des
        vmovdqa xmmword ptr [ebx],xmm1      ;save indices (unchanged)
        vmovdqa ymmword ptr [ecx],ymm2      ;save mask (all zeros)

        vzeroupper
        pop ebx
        pop ebp
        ret
AvxGatherI64_ endp
        end
