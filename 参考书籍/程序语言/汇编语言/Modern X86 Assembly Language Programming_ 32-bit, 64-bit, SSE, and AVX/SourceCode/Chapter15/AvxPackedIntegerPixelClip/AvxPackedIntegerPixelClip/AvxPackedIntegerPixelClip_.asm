        .model flat,c

; The following structure must match the stucture that's declared
; in the file AvxPackedIntegerPixelClip.h.

PcData              struct
Src                 dword ?             ;source buffer
Des                 dword ?             ;destination buffer
NumPixels           dword ?             ;number of pixels
NumClippedPixels    dword ?             ;number of clipped pixels
ThreshLo            byte ?              ;low threshold
ThreshHi            byte ?              ;high threshold
PcData              ends

; Custom segment for constant values
PcConstVals segment readonly align(32) public

PixelScale  byte 32 dup(80h)            ;Pixel Uint8 to Int8 scale value

; The following values defined are for illustrative purposes only
; Note that the align 32 directive does not work in a .const section
Test1       dword 10
Test2       qword -20
            align 32
Test3       byte 32 dup(7fh)
PcConstVals ends
            .code

; extern "C" bool AvxPiPixelClip_(PcData* pc_data);
;
; Description:  The following function clips the pixels of an image
;               buffer to values between ThreshLo and ThreshHi.
;
; Requires:     AVX2, POPCNT

AvxPiPixelClip_ proc
        push ebp
        mov ebp,esp
        push ebx
        push esi
        push edi

; Load and validate arguments
        xor eax,eax
        mov ebx,[ebp+8]                     ;ebx = pc_data
        mov ecx,[ebx+PcData.NumPixels]      ;ecx = num_pixels
        cmp ecx,32
        jl BadArg                           ;jump if num_pixels < 32
        test ecx,1fh
        jnz BadArg                          ;jump if num_pixels % 32 != 0

        mov esi,[ebx+PcData.Src]            ;esi = Src
        test esi,1fh
        jnz BadArg                          ;jump if Src is misaligned

        mov edi,[ebx+PcData.Des]            ;edi = Des
        test edi,1fh
        jnz BadArg                          ;jump if Des is misaligned

; Create packed thresh_lo and thresh_hi data values
        vmovdqa ymm5,ymmword ptr [PixelScale]

        vpbroadcastb ymm0,[ebx+PcData.ThreshLo]     ;ymm0 = thresh_lo
        vpbroadcastb ymm1,[ebx+PcData.ThreshHi]     ;ymm1 = thresh_hi

        vpsubb ymm6,ymm0,ymm5               ;ymm6 = scaled thresh_lo
        vpsubb ymm7,ymm1,ymm5               ;ymm7 = scaled thresh_hi

        xor edx,edx                         ;edx = num_clipped_pixels
        shr ecx,5                           ;ecx = number of 32-byte blocks

; Sweep through the image buffer and clip pixels to threshold values
@@:     vmovdqa ymm0,ymmword ptr [esi]      ;ymmo = unscaled pixels
        vpsubb ymm0,ymm0,ymm5               ;ymm0 = scaled pixels

        vpcmpgtb ymm1,ymm0,ymm7             ;mask of pixels GT thresh_hi
        vpand ymm2,ymm1,ymm7                ;new values for GT pixels

        vpcmpgtb ymm3,ymm6,ymm0             ;mask of pixels LT thresh_lo
        vpand ymm4,ymm3,ymm6                ;new values for LT pixels

        vpor ymm1,ymm1,ymm3                 ;mask of all clipped pixels

        vpor ymm2,ymm2,ymm4                 ;clipped pixels
        vpandn ymm3,ymm1,ymm0               ;unclipped pixels

        vpor ymm4,ymm3,ymm2                 ;final scaled clipped pixels
        vpaddb ymm4,ymm4,ymm5               ;final unscaled clipped pixels

        vmovdqa ymmword ptr [edi],ymm4      ;save clipped pixels

; Update num_clipped_pixels
        vpmovmskb eax,ymm1                  ;eax = clipped pixel mask
        popcnt eax,eax                      ;count clipped pixels
        add edx,eax                         ;update num_clipped_pixels
        add esi,32
        add edi,32
        dec ecx
        jnz @B

; Save num_clipped_pixels
        mov eax,1                           ;set success return code
        mov [ebx+PcData.NumClippedPixels],edx
        vzeroupper

BadArg: pop edi
        pop esi
        pop ebx
        pop ebp
        ret
AvxPiPixelClip_ endp
        end
