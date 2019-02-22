        .model flat,c

; This structure must match the structure that's defined
; in AvxCpuid.cpp

CpuidRegs   struct
RegEAX      dword ?
RegEBX      dword ?
RegECX      dword ?
RegEDX      dword ?
CpuidRegs   ends
            .code

; extern "C" Uint32 Cpuid_(Uint32 r_eax, Uint32 r_ecx, CpuidRegs* r_out);
;
; Description:  The following function uses the CPUID instruction to
;               query processor identification and feature information.
;
; Returns:      eax == 0     Unsupported CPUID leaf
;               eax != 0     Supported CPUID leaf
;
;               The return code is valid only if r_eax <= MaxEAX.

Cpuid_  proc
        push ebp
        mov ebp,esp
        push ebx
        push esi

; Load eax and ecx with provided values, then use cpuid
        mov eax,[ebp+8]
        mov ecx,[ebp+12]
        cpuid

; Save results
        mov esi,[ebp+16]
        mov [esi+CpuidRegs.RegEAX],eax
        mov [esi+CpuidRegs.RegEBX],ebx
        mov [esi+CpuidRegs.RegECX],ecx
        mov [esi+CpuidRegs.RegEDX],edx

; Test for unsupported CPUID leaf
        or eax,ebx
        or ecx,edx
        or eax,ecx                          ;eax = return code

        pop esi
        pop ebx
        pop ebp
        ret
Cpuid_  endp

; extern "C" void Xgetbv_(Uint32 r_ecx, Uint32* r_eax, Uint32* r_edx);
;
; Description:  The following function uses the XGETBV instruction to
;               obtain the contents of the extended control register
;               that's specified by r_ecx.
;
; Notes:        A processor exception will occur if r_ecx is invalid
;               or if the XSAVE feature set is disabled.

Xgetbv_  proc
        push ebp
        mov ebp,esp

        mov ecx,[ebp+8]                     ;ecx = extended control reg
        xgetbv

        mov ecx,[ebp+12]
        mov [ecx],eax                       ;save result (low dword)
        mov ecx,[ebp+16]
        mov [ecx],edx                       ;save result (high dword)

        pop ebp
        ret
Xgetbv_ endp
        end
