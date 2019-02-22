        .model flat,c
        .code

; extern "C" void AvxSfpCompare_(double a, double b, bool results[8]);
;
; Description:  The following function demonstrates use of the
;               x86-AVX compare instruction vcmpsd.
;
; Requires:     AVX

AvxSfpCompare_ proc
        push ebp
        mov ebp,esp

; Load argument values
        vmovsd xmm0,real8 ptr [ebp+8]       ;xmm0 = a
        vmovsd xmm1,real8 ptr [ebp+16]      ;xmm1 = b;
        mov eax,[ebp+24]                    ;eax= ptr to results array

; Perform compare for equality
        vcmpeqsd xmm2,xmm0,xmm1             ;perform compare operation
        vmovmskpd ecx,xmm2                  ;move result to bit 0 of ecx
        test ecx,1                          ;test bit result
        setnz byte ptr [eax+0]              ;save result as C++ bool

; Perform compare for inequality.  Note that vcmpneqsd returns true
; if used with QNaN or SNaN operand values.
        vcmpneqsd xmm2,xmm0,xmm1
        vmovmskpd ecx,xmm2
        test ecx,1
        setnz byte ptr [eax+1]

; Perform compare for less than
        vcmpltsd xmm2,xmm0,xmm1
        vmovmskpd ecx,xmm2
        test ecx,1
        setnz byte ptr [eax+2]

; Perform compare for less than or equal
        vcmplesd xmm2,xmm0,xmm1
        vmovmskpd ecx,xmm2
        test ecx,1
        setnz byte ptr [eax+3]

; Perform compare for greater than
        vcmpgtsd xmm2,xmm0,xmm1
        vmovmskpd ecx,xmm2
        test ecx,1
        setnz byte ptr [eax+4]

; Perform compare for greater than or equal
        vcmpgesd xmm2,xmm0,xmm1
        vmovmskpd ecx,xmm2
        test ecx,1
        setnz byte ptr [eax+5]

; Perform compare for ordered
        vcmpordsd xmm2,xmm0,xmm1
        vmovmskpd ecx,xmm2
        test ecx,1
        setnz byte ptr [eax+6]

; Perform compare for unordered
        vcmpunordsd xmm2,xmm0,xmm1
        vmovmskpd ecx,xmm2
        test ecx,1
        setnz byte ptr [eax+7]

        pop ebp
        ret
AvxSfpCompare_ endp
        end
