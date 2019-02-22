        .model flat,c
         extern NUM_PIXELS_MAX:dword

; Image threshold data structure (see AvxPackedIntegerThreshold.h)
ITD                 struct
PbSrc               dword ?
PbMask              dword ?
NumPixels           dword ?
Threshold           byte ?
Pad                 byte 3 dup(?)
NumMaskedPixels     dword ?
SumMaskedPixels     dword ?
MeanMaskedPixels    real8 ?
ITD                 ends

; Custom segment for constant values
ItConstVals segment readonly align(32) public
PixelScale      byte 32 dup(80h)            ;uint8 to int8 scale value
R8_MinusOne     real8 -1.0                  ;invalid mean value
ItConstVals     ends

                .code

; extern "C" bool AvxPiThreshold_(ITD* itd);
;
; Description:  The following function performs image thresholding
;               of an 8 bits-per-pixel grayscale image.
;
; Returns:      0 = invalid size or unaligned image buffer
;               1 = success
;
; Requires:     AVX2

AvxPiThreshold_ proc
        push ebp
        mov ebp,esp
        push esi
        push edi

; Load and verify the argument values in ITD structure
        mov edx,[ebp+8]                     ;edx = 'itd'
        xor eax,eax                         ;set error return code
        mov ecx,[edx+ITD.NumPixels]         ;ecx = NumPixels
        test ecx,ecx
        jz Done                             ;jump if num_pixels == 0
        cmp ecx,[NUM_PIXELS_MAX]
        ja Done                             ;jump if num_pixels too big
        test ecx,1fh
        jnz Done                            ;jump if num_pixels % 32 != 0
        shr ecx,5                           ;ecx = number of packed pixels

        mov esi,[edx+ITD.PbSrc]             ;esi = PbSrc
        test esi,1fh
        jnz Done                            ;jump if misaligned
        mov edi,[edx+ITD.PbMask]            ;edi = PbMask
        test edi,1fh
        jnz Done                            ;jump if misaligned

; Initialize packed threshold
        vpbroadcastb ymm0,[edx+ITD.Threshold]   ;ymm0 = packed threshold
        vmovdqa ymm7,ymmword ptr [PixelScale]   ;ymm7 = uint8 to int8 SF
        vpsubb ymm2,ymm0,ymm7                   ;ymm1 = scaled threshold

; Create the mask image
@@:     vmovdqa ymm0,ymmword ptr [esi]      ;load next packed pixel
        vpsubb ymm1,ymm0,ymm7               ;ymm1 = scaled image pixels
        vpcmpgtb ymm3,ymm1,ymm2             ;compare against threshold
        vmovdqa ymmword ptr [edi],ymm3      ;save packed threshold mask

        add esi,32
        add edi,32
        dec ecx
        jnz @B                              ;repeat until done
        mov eax,1                           ;set return code

Done:   pop edi
        pop esi
        pop ebp
        ret
AvxPiThreshold_ endp

; Marco AvxPiCalcMeanUpdateSums
;
; Description:  The following macro updates sum_masked_pixels in ymm4.
;               It also resets any necessary intermediate values in
;               order to prevent an overflow condition.
;
; Register contents:
;   ymm3:ymm2 = packed word sum_masked_pixels
;   ymm4 = packed dword sum_masked_pixels
;   ymm7 = packed zero
;
; Temp registers:
;   ymm0, ymm1, ymm5, ymm6

AvxPiCalcMeanUpdateSums macro

; Promote packed word sum_masked_pixels to dword
        vpunpcklwd ymm0,ymm2,ymm7
        vpunpcklwd ymm1,ymm3,ymm7
        vpunpckhwd ymm5,ymm2,ymm7
        vpunpckhwd ymm6,ymm3,ymm7

; Update packed dword sums in sum_masked_pixels
        vpaddd ymm0,ymm0,ymm1
        vpaddd ymm5,ymm5,ymm6
        vpaddd ymm4,ymm4,ymm0
        vpaddd ymm4,ymm4,ymm5

; Reset intermediate values
        xor edx,edx                         ;reset update counter
        vpxor ymm2,ymm2,ymm2                ;reset sum_masked_pixels lo
        vpxor ymm3,ymm3,ymm3                ;reset sum_masked_pixels hi
        endm

; extern "C" bool AvxPiCalcMean_(ITD* itd);
;
; Description:  The following function calculates the mean value all
;               above-threshold image pixels using the mask created by
;               function AvxPiThreshold_.
;
; Returns:      0 = invalid image size or unaligned image buffer
;               1 = success
;
; Requires:     AVX2, POPCNT

AvxPiCalcMean_  proc
        push ebp
        mov ebp,esp
        push ebx
        push esi
        push edi

; Load and verify the argument values in ITD structure
        mov eax,[ebp+8]                     ;eax = 'itd'
        mov ecx,[eax+ITD.NumPixels]         ;ecx = NumPixels
        test ecx,ecx
        jz Error                            ;jump if num_pixels == 0
        cmp ecx,[NUM_PIXELS_MAX]
        ja Error                            ;jump if num_pixels too big
        test ecx,1fh
        jnz Error                           ;jump if num_pixels % 32 != 0
        shr ecx,5                           ;ecx = number of packed pixels

        mov edi,[eax+ITD.PbMask]            ;edi = PbMask
        test edi,1fh
        jnz Error                           ;jump if PbMask not aligned
        mov esi,[eax+ITD.PbSrc]             ;esi = PbSrc
        test esi,1fh
        jnz Error                           ;jump if PbSrc not aligned

; Initialize values for mean calculation
        xor edx,edx                 ;edx = update counter
        vpxor ymm7,ymm7,ymm7        ;ymm7 = packed zero
        vmovdqa ymm2,ymm7           ;ymm2 = sum_masked_pixels (16 words)
        vmovdqa ymm3,ymm7           ;ymm3 = sum_masked_pixels (16 words)
        vmovdqa ymm4,ymm7           ;ymm4 = sum_masked_pixels (8 dwords)
        xor ebx,ebx                 ;ebx = num_masked_pixels (1 dword)

; Register usage for processing loop
; esi = PbSrc, edi = PbMask, eax = scratch register
; ebx = num_pixels_masked, ecx = NumPixels / 32, edx = update counter
;
; ymm0 = packed pixel, ymm1 = packed mask
; ymm3:ymm2 = sum_masked_pixels (32 words)
; ymm4 = sum_masked_pixels (8 dwords)
; ymm5 = scratch register
; ymm6 = scratch register
; ymm7 = packed zero

@@:     vmovdqa ymm0,ymmword ptr [esi]      ;load next packed pixel
        vmovdqa ymm1,ymmword ptr [edi]      ;load next packed mask

; Update mum_masked_pixels
        vpmovmskb eax,ymm1
        popcnt eax,eax
        add ebx,eax

; Update sum_masked_pixels (word values)
        vpand ymm6,ymm0,ymm1            ;set non-masked pixels to zero
        vpunpcklbw ymm0,ymm6,ymm7
        vpunpckhbw ymm1,ymm6,ymm7       ;ymm1:ymm0 = masked pixels (words)
        vpaddw ymm2,ymm2,ymm0
        vpaddw ymm3,ymm3,ymm1           ;ymm3:ymm2 = sum_masked_pixels

; Check and see if it's necessary to update the dword sum_masked_pixels
; in xmm4 and num_masked_pixels in ebx
        inc edx
        cmp edx,255
        jb NoUpdate
        AvxPiCalcMeanUpdateSums
NoUpdate:
        add esi,32
        add edi,32
        dec ecx
        jnz @B                              ;repeat loop until done

; Main processing loop is finished. If necessary, perform final update
; of sum_masked_pixels in xmm4 & num_masked_pixels in ebx.
        test edx,edx
        jz @F
        AvxPiCalcMeanUpdateSums

; Compute and save final sum_masked_pixels & num_masked_pixels
@@:     vextracti128 xmm0,ymm4,1
        vpaddd xmm1,xmm0,xmm4
        vphaddd xmm2,xmm1,xmm7
        vphaddd xmm3,xmm2,xmm7
        vmovd edx,xmm3                      ;edx = final sum_mask_pixels

        mov eax,[ebp+8]                     ;eax = 'itd'
        mov [eax+ITD.SumMaskedPixels],edx   ;save final sum_masked_pixels
        mov [eax+ITD.NumMaskedPixels],ebx   ;save final num_masked_pixels

; Compute mean of masked pixels
        test ebx,ebx                        ;is num_mask_pixels zero?
        jz NoMean                           ;if yes, skip calc of mean
        vcvtsi2sd xmm0,xmm0,edx             ;xmm0 = sum_masked_pixels
        vcvtsi2sd xmm1,xmm1,ebx             ;xmm1 = num_masked_pixels
        vdivsd xmm0,xmm0,xmm1               ;xmm0 = mean_masked_pixels
        jmp @F
NoMean: vmovsd xmm0,[R8_MinusOne]               ;use -1.0 for no mean
@@:     vmovsd [eax+ITD.MeanMaskedPixels],xmm0  ;save mean
        mov eax,1                               ;set return code
        vzeroupper

Done:   pop edi
        pop esi
        pop ebx
        pop ebp
        ret

Error:  xor eax,eax                         ;set error return code
        jmp Done
AvxPiCalcMean_  endp
        end
