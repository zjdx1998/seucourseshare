        .model flat,c
        .code

; extern "C" void AvxBroadcastIntegerYmm_(YmmVal* des, const XmmVal* src, Brop op);
;
; Description:  The following function demonstrates use of the
;               vpbroadcastX instruction.
;
; Requires:     AVX2

AvxBroadcastIntegerYmm_ proc
        push ebp
        mov ebp,esp

; Make sure op is valid
        mov eax,[ebp+16]                ;eax = op
        cmp eax,BropTableCount
        jae BadOp                       ;jump if op is invalid

; Load parameters and jump to specified instruction
        mov ecx,[ebp+8]                 ;ecx = des
        mov edx,[ebp+12]                ;edx = src
        vmovdqa xmm0,xmmword ptr [edx]  ;xmm0 = broadcast value (low item)
        mov edx,[BropTable+eax*4]
        jmp edx

; Perform byte broadcast
BropByte:
        vpbroadcastb ymm1,xmm0
        vmovdqa ymmword ptr [ecx],ymm1
        vzeroupper
        pop ebp
        ret

; Perform word broadcast
BropWord:
        vpbroadcastw ymm1,xmm0
        vmovdqa ymmword ptr [ecx],ymm1
        vzeroupper
        pop ebp
        ret

; Perform dword broadcast
BropDword:
        vpbroadcastd ymm1,xmm0
        vmovdqa ymmword ptr [ecx],ymm1
        vzeroupper
        pop ebp
        ret

; Perform qword broadcast
BropQword:
        vpbroadcastq ymm1,xmm0
        vmovdqa ymmword ptr [ecx],ymm1
        vzeroupper
        pop ebp
        ret

BadOp:  pop ebp
        ret

; The order of values in the following table must match the enum Brop
; that's defined in AvxBroadcast.cpp.

        align 4
BropTable dword BropByte, BropWord, BropDword, BropQword
BropTableCount equ ($ - BropTable) / size dword

AvxBroadcastIntegerYmm_ endp

; extern "C" void AvxBroadcastFloat_(YmmVal* des, float val);
;
; Description:  The following function demonstrates use of the
;               vbroadcastss instruction.
;
; Requires:     AVX

AvxBroadcastFloat_ proc
        push ebp
        mov ebp,esp

; Broadcast val to all elements of des
        mov eax,[ebp+8]
        vbroadcastss ymm0,real4 ptr [ebp+12]
        vmovaps ymmword ptr [eax],ymm0

        vzeroupper
        pop ebp
        ret
AvxBroadcastFloat_ endp

; extern "C" void AvxBroadcastDouble_(YmmVal* des, double val);
;
; Description:  The following function demonstrates use of the
;               vbroadcastsd instruction.
;
; Requires:     AVX

AvxBroadcastDouble_ proc
        push ebp
        mov ebp,esp

; Broadcast val to all elements of des.
        mov eax,[ebp+8]
        vbroadcastsd ymm0,real8 ptr [ebp+12]
        vmovapd ymmword ptr [eax],ymm0

        vzeroupper
        pop ebp
        ret
AvxBroadcastDouble_ endp
        end
