        include <MacrosX86-64.inc>

; 256-bit wide constants
ConstVals segment readonly align(32)
InitialPminVal db 32 dup(0ffh)
InitialPmaxVal db 32 dup(00h)
ConstVals ends
        .code

; Macro _YmmVpextrMinub
;
; Description:  The following macro generates code that extracts the
;               smallest unsigned byte value from register YmmSrc.

_YmmVpextrMinub macro GprDes,YmmSrc,YmmTmp

; Make sure YmmSrc and YmmTmp are different
.erridni <YmmSrc>, <YmmTmp>, <Invalid registers>

; Construct text strings for the corresponding XMM registers
        YmmSrcSuffix SUBSTR <YmmSrc>,2
        XmmSrc CATSTR <X>,YmmSrcSuffix

        YmmTmpSuffix SUBSTR <YmmTmp>,2
        XmmTmp CATSTR <X>,YmmTmpSuffix

; Reduce the 32 byte values in YmmSrc to the smallest value
        vextracti128 XmmTmp,YmmSrc,1
        vpminub XmmSrc,XmmSrc,XmmTmp        ;XmmSrc = final 16 min values

        vpsrldq XmmTmp,XmmSrc,8
        vpminub XmmSrc,XmmSrc,XmmTmp        ;XmmSrc = final 8 min values

        vpsrldq XmmTmp,XmmSrc,4
        vpminub XmmSrc,XmmSrc,XmmTmp        ;XmmSrc = final 4 min values

        vpsrldq XmmTmp,XmmSrc,2
        vpminub XmmSrc,XmmSrc,XmmTmp        ;XmmSrc = final 2 min values

        vpsrldq XmmTmp,XmmSrc,1
        vpminub XmmSrc,XmmSrc,XmmTmp        ;XmmSrc = final 1 min value

        vpextrb GprDes,XmmSrc,0             ;mov final min value to Gpr
        endm

; Macro _YmmVpextrMaxub
;
; Description:  The following macro generates code that extracts the
;               largest unsigned byte value from register YmmSrc.

_YmmVpextrMaxub macro GprDes,YmmSrc,YmmTmp

; Make sure YmmSrc and YmmTmp are different
.erridni <YmmSrc>, <YmmTmp>, <Invalid registers>

; Construct text strings for the corresponding XMM registers
        YmmSrcSuffix SUBSTR <YmmSrc>,2
        XmmSrc CATSTR <X>,YmmSrcSuffix

        YmmTmpSuffix SUBSTR <YmmTmp>,2
        XmmTmp CATSTR <X>,YmmTmpSuffix

; Reduce the 32 byte values in YmmSrc to the largest value
        vextracti128 XmmTmp,YmmSrc,1
        vpmaxub XmmSrc,XmmSrc,XmmTmp        ;XmmSrc = final 16 max values

        vpsrldq XmmTmp,XmmSrc,8
        vpmaxub XmmSrc,XmmSrc,XmmTmp        ;XmmSrc = final 8 max values

        vpsrldq XmmTmp,XmmSrc,4
        vpmaxub XmmSrc,XmmSrc,XmmTmp        ;XmmSrc = final 4 max values

        vpsrldq XmmTmp,XmmSrc,2
        vpmaxub XmmSrc,XmmSrc,XmmTmp        ;XmmSrc = final 2 max values

        vpsrldq XmmTmp,XmmSrc,1
        vpmaxub XmmSrc,XmmSrc,XmmTmp        ;XmmSrc = final 1 max value

        vpextrb GprDes,XmmSrc,0             ;mov final max value to Gpr
        endm

; extern "C" bool Avx64CalcRgbMinMax_(Uint8* rgb[3], Uint32 num_pixels, Uint8 min_vals[3], Uint8 max_vals[3]);
;
; Description:  The following function determines the minimum and maximum
;               pixel values of each color plane array.
;
; Requires:     x86-64, AVX2

Avx64CalcRgbMinMax_ proc frame
        _CreateFrame CalcMinMax_,0,48,r12
        _SaveXmmRegs xmm6,xmm7,xmm8
        _EndProlog

; Make sure num_pixels and the color plane arrays are valid
        test edx,edx
        jz Error                            ;jump if num_pixels == 0
        test edx,01fh
        jnz Error                           ;jump if num_pixels % 32 != 0

        xor rax,rax
        mov r10,[rcx]                       ;r10 = R color plane ptr
        or rax,r10
        mov r11,[rcx+8]                     ;r11 = G color plane ptr
        or rax,r11
        mov r12,[rcx+16]                    ;r12 = B color plane ptr
        or rax,r12
        test rax,1fh
        jnz Error                           ;jump if R, G, or B misaligned

; Initialize the processing loop registers
        shr edx,5                           ;edx = number of pixel blocks
        xor rcx,rcx                         ;rcx = common array offset

        vmovdqa ymm3,ymmword ptr [InitialPminVal]   ;ymm3 = R minimums
        vmovdqa ymm4,ymm3                           ;ymm4 = G minimums
        vmovdqa ymm5,ymm3                           ;ymm5 = B minimums

        vmovdqa ymm6,ymmword ptr [InitialPmaxVal]   ;ymm6 = R maximums
        vmovdqa ymm7,ymm6                           ;ymm7 = G maximums
        vmovdqa ymm8,ymm6                           ;ymm8 = B maximums

; Scan RGB color plane arrays for packed minimums and maximums
@@:     vmovdqa ymm0,ymmword ptr [r10+rcx]  ;ymm0 = R pixels
        vmovdqa ymm1,ymmword ptr [r11+rcx]  ;ymm1 = G pixels
        vmovdqa ymm2,ymmword ptr [r12+rcx]  ;ymm2 = B pixels

        vpminub ymm3,ymm3,ymm0              ;update R minimums
        vpminub ymm4,ymm4,ymm1              ;update G minimums
        vpminub ymm5,ymm5,ymm2              ;update B minmums

        vpmaxub ymm6,ymm6,ymm0              ;update R maximums
        vpmaxub ymm7,ymm7,ymm1              ;update G maximums
        vpmaxub ymm8,ymm8,ymm2              ;update B maximums

        add rcx,32
        sub edx,1
        jnz @B

; Calculate the final RGB minimum values
        _YmmVpextrMinub rax,ymm3,ymm0
        mov byte ptr [r8],al                ;save min R
        _YmmVpextrMinub rax,ymm4,ymm0
        mov byte ptr [r8+1],al              ;save min G
        _YmmVpextrMinub rax,ymm5,ymm0
        mov byte ptr [r8+2],al              ;save min B

; Calculate the final RGB maximum values
        _YmmVpextrMaxub rax,ymm6,ymm1
        mov byte ptr [r9],al                ;save max R
        _YmmVpextrMaxub rax,ymm7,ymm1
        mov byte ptr [r9+1],al              ;save max G
        _YmmVpextrMaxub rax,ymm8,ymm1
        mov byte ptr [r9+2],al              ;save max B

        mov eax,1                           ;set success return code
        vzeroupper

Done:   _RestoreXmmRegs xmm6,xmm7,xmm8
        _DeleteFrame r12
        ret

Error:  xor eax,eax                         ;set error return code
        jmp Done
Avx64CalcRgbMinMax_ endp
        end
