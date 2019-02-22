        .model flat,c
        .code
        extern NMIN:dword, NMAX:dword       ;min and max array sizes

; extern "C" bool MmxCalcMean_(const Uint8* x, int n, Uint32* sum_x, double* mean);
;
; Description:  This function calculates the sum and mean of an array
;               containing 8-bit unsigned integers.
;
; Returns       0 = invalid 'n'
;               1 = success

MmxCalcMean_ proc
        push ebp
        mov ebp,esp
        sub esp,8                       ;local storage for x87 transfer

; Verify n is valid
        xor eax,eax                     ;set error return code
        mov ecx,[ebp+12]
        cmp ecx,[NMIN]
        jl Done                         ;jump if n < NMIN
        cmp ecx,[NMAX]
        jg Done                         ;jump if n > NMAX
        test ecx,0fh
        jnz Done                        ;jump if n % 16 != 0
        shr ecx,4                       ;number of 16-byte blocks

; Perform required initializations
        mov eax,[ebp+8]                 ;pointer to array 'x'
        pxor mm4,mm4
        pxor mm5,mm5                    ;mm5:mm4 = packed sum (4 dwords)
        pxor mm7,mm7                    ;mm7 = packed zero for promotions

; Load the next block of 16 array values 
@@:     movq mm0,[eax]
        movq mm1,[eax+8]                    ;mm1:mm0 = 16 byte block

; Promote array values from bytes to words, then sum the words
        movq mm2,mm0
        movq mm3,mm1
        punpcklbw mm0,mm7                   ;mm0 = 4 words
        punpcklbw mm1,mm7                   ;mm1 = 4 words
        punpckhbw mm2,mm7                   ;mm2 = 4 words
        punpckhbw mm3,mm7                   ;mm3 = 4 words
        paddw mm0,mm2
        paddw mm1,mm3
        paddw mm0,mm1                       ;mm0 = pack sums (4 words)

; Promote packed sums to dwords, then update dword sums in mm5:mm4
        movq mm1,mm0
        punpcklwd mm0,mm7               ;mm0 = packed sums (2 dwords)
        punpckhwd mm1,mm7               ;mm1 = packed sums (2 dwords)
        paddd mm4,mm0
        paddd mm5,mm1                   ;mm5:mm4 = packed sums (4 dwords)

        add eax,16                          ;eax = next 16 byte block
        dec ecx
        jnz @B                              ;repeat loop if not done

; Compute final sum_x
        paddd mm5,mm4                       ;mm5 = packed sums (2 dwords)
        pshufw mm6,mm5,00001110b            ;mm6[31:0] = mm5[63:32]
        paddd mm6,mm5                       ;mm6[31:0] = final sum_x
        movd eax,mm6                        ;eax = sum_x
        emms                                ;clear mmx state

; Compute mean value
        mov dword ptr [ebp-8],eax           ;save sum_x as 64-bit value
        mov dword ptr [ebp-4],0
        fild qword ptr [ebp-8]              ;load sum_x
        fild dword ptr [ebp+12]             ;load n
        fdivp                               ;mean = sum_x / n

        mov edx,[ebp+20]
        fstp real8 ptr [edx]                ;save mean
        mov edx,[ebp+16]
        mov [edx],eax                       ;save sum_x
        mov eax,1                           ;set return code

Done:   mov esp,ebp
        pop ebp
        ret
MmxCalcMean_ endp
        end
