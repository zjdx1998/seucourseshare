        .model flat,c
        .code

; void AvxFmaSmooth5a_(float* y, const float*x, Uint32 n, const float* sm5_mask);
;
; Description:  The following function applies a weighted-average
;               smoothing transformation to the input array x using
;               scalar SPFP multiplication and addition.
;
; Requires:     AVX

AvxFmaSmooth5a_ proc
        push ebp
        mov ebp,esp
        push esi
        push edi

; Load argument values
        mov edi,[ebp+8]                     ;edi = ptr to y
        mov esi,[ebp+12]                    ;esi = ptr to x
        mov ecx,[ebp+16]                    ;ecx = n
        mov eax,[ebp+20]                    ;eax = ptr to sm5_mask

        add esi,8                           ;adjust pointers and
        add edi,8                           ;counter to skip first 2
        sub ecx,4                           ;and last 2 elements
        align 16

; Apply smoothing operator to each element of x
@@:     vxorps xmm6,xmm6,xmm6               ;x_total=0

; Compute x_total += x[i-2]*sm5_mask[0]
        vmovss xmm0,real4 ptr [esi-8]
        vmulss xmm1,xmm0,real4 ptr [eax]
        vaddss xmm6,xmm6,xmm1

; Compute x_total += x[i-1]*sm5_mask[1]
        vmovss xmm2,real4 ptr [esi-4]
        vmulss xmm3,xmm2,real4 ptr [eax+4]
        vaddss xmm6,xmm6,xmm3

; Compute x_total += x[i]*sm5_mask[2]
        vmovss xmm0,real4 ptr [esi]
        vmulss xmm1,xmm0,real4 ptr [eax+8]
        vaddss xmm6,xmm6,xmm1

; Compute x_total += x[i+1]*sm5_mask[3]
        vmovss xmm2,real4 ptr [esi+4]
        vmulss xmm3,xmm2,real4 ptr [eax+12]
        vaddss xmm6,xmm6,xmm3

; Compute x_total += x[i+2]*sm5_mask[4]
        vmovss xmm0,real4 ptr [esi+8]
        vmulss xmm1,xmm0,real4 ptr [eax+16]
        vaddss xmm6,xmm6,xmm1

; Save x_total
        vmovss real4 ptr [edi],xmm6

        add esi,4
        add edi,4
        sub ecx,1
        jnz @B

        pop edi
        pop esi
        pop ebp
        ret
AvxFmaSmooth5a_ endp

; void AvxFmaSmooth5b_(float* y, const float*x, Uint32 n, const float* sm5_mask);
;
; Description:  The following function applies a weighted-average
;               smoothing transformation to the input array x using
;               scalar SPFP fused-multiply-add operations.
;
; Requires:     AVX2, FMA

AvxFmaSmooth5b_ proc
        push ebp
        mov ebp,esp
        push esi
        push edi

; Load argument values
        mov edi,[ebp+8]                     ;edi = ptr to y
        mov esi,[ebp+12]                    ;esi = ptr to x
        mov ecx,[ebp+16]                    ;ecx = n
        mov eax,[ebp+20]                    ;eax = ptr to sm5_mask

        add esi,8                           ;adjust pointers and
        add edi,8                           ;counter to skip first 2
        sub ecx,4                           ;and last 2 elements
        align 16

; Apply smoothing operator to each element of x
@@:     vxorps xmm6,xmm6,xmm6               ;set x_total1 = 0
        vxorps xmm7,xmm7,xmm7               ;set x_total2 = 0

; Compute x_total1 = x[i-2] * sm5_mask[0] + x_total1
        vmovss xmm0,real4 ptr [esi-8]
        vfmadd231ss xmm6,xmm0,real4 ptr [eax]

; Compute x_total2 = x[i-1] * sm5_mask[1] + x_total2
        vmovss xmm2,real4 ptr [esi-4]
        vfmadd231ss xmm7,xmm2,real4 ptr [eax+4]

; Compute x_total1 = x[i] * sm5_mask[2] + x_total1
        vmovss xmm0,real4 ptr [esi]
        vfmadd231ss xmm6,xmm0,real4 ptr [eax+8]

; Compute x_total2 = x[i+1] * sm5_mask[3] + x_total2
        vmovss xmm2,real4 ptr [esi+4]
        vfmadd231ss xmm7,xmm2,real4 ptr [eax+12]

; Compute x_total1 = x[i+2] * sm5_mask[4] + x_total1
        vmovss xmm0,real4 ptr [esi+8]
        vfmadd231ss xmm6,xmm0,real4 ptr [eax+16]

; Compute final x_total and save result
        vaddss xmm5,xmm6,xmm7
        vmovss real4 ptr [edi],xmm5

        add esi,4
        add edi,4
        sub ecx,1
        jnz @B

        pop edi
        pop esi
        pop ebp
        ret
AvxFmaSmooth5b_ endp

; void AvxFmaSmooth5c_(float* y, const float*x, Uint32 n, const float* sm5_mask);
;
; Description:  The following function applies a weighted-average
;               smoothing transformation to the input array x using
;               scalar SPFP fused-multiply-add operations.
;
; Requires:     AVX2, FMA

AvxFmaSmooth5c_ proc
        push ebp
        mov ebp,esp
        push esi
        push edi

; Load argument values
        mov edi,[ebp+8]                     ;edi = ptr to y
        mov esi,[ebp+12]                    ;esi = ptr to x
        mov ecx,[ebp+16]                    ;ecx = n
        mov eax,[ebp+20]                    ;eax = ptr to sm5_mask

        add esi,8                           ;adjust pointers and
        add edi,8                           ;counter to skip first 2
        sub ecx,4                           ;and last 2 elements
        align 16

; Apply smoothing operator to each element of x, save result to y
@@:     vxorps xmm6,xmm6,xmm6               ;set x_total = 0

; Compute x_total = x[i-2] * sm5_mask[0] + x_total
        vmovss xmm0,real4 ptr [esi-8]
        vfmadd231ss xmm6,xmm0,real4 ptr [eax]

; Compute x_total = x[i-1] * sm5_mask[1] + x_total
        vmovss xmm0,real4 ptr [esi-4]
        vfmadd231ss xmm6,xmm0,real4 ptr [eax+4]

; Compute x_total = x[i] * sm5_mask[2] + x_total
        vmovss xmm0,real4 ptr [esi]
        vfmadd231ss xmm6,xmm0,real4 ptr [eax+8]

; Compute x_total = x[i+1] * sm5_mask[3] + x_total
        vmovss xmm0,real4 ptr [esi+4]
        vfmadd231ss xmm6,xmm0,real4 ptr [eax+12]

; Compute x_total = x[i+2] * sm5_mask[4] + x_total
        vmovss xmm0,real4 ptr [esi+8]
        vfmadd231ss xmm6,xmm0,real4 ptr [eax+16]

; Save result
        vmovss real4 ptr [edi],xmm6

        add esi,4
        add edi,4
        sub ecx,1
        jnz @B

        pop edi
        pop esi
        pop ebp
        ret
AvxFmaSmooth5c_ endp
        end
