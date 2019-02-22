        .model flat,c
        .code

; extern "C" bool AvxPfpColMeans_(const double* x, int nrows, int ncols, double* col_means)
;
; Description:  The following function computes the mean value of each
;               column in a matrix of DPFP values.
;
; Requires:     AVX

AvxPfpColMeans_ proc
        push ebp
        mov ebp,esp
        push ebx
        push esi
        push edi

; Load and validate arguments
        mov esi,[ebp+8]                     ;esi = ptr to x

        xor eax,eax
        mov edx,[ebp+12]                    ;edx = nrows
        test edx,edx
        jle BadArg                          ;jump if nrows <= 0

        mov ecx,[ebp+16]                    ;ecx = ncols
        test ecx,ecx
        jle BadArg                          ;jump if ncols <= 0

        mov edi,[ebp+20]                    ;edi = ptr to col_means
        test edi,1fh
        jnz BadArg                          ;jump if col_means not aligned

; Set col_means to zero
        mov ebx,ecx                         ;ebx = ncols
        shl ecx,1                           ;ecx = num dowrds in col_means
        rep stosd                           ;set col_means to zero

; Compute the sum of each column in x
LP1:    mov edi,[ebp+20]                    ;edi = ptr to col_means
        xor ecx,ecx                         ;ecx = col_index

LP2:    mov eax,ecx                         ;eax = col_index
        add eax,4
        cmp eax,ebx                         ;4 or more columns remaining?
        jg @F                               ;jump if col_index + 4 > ncols

; Update col_means using next four columns
        vmovupd ymm0,ymmword ptr [esi]      ;load next 4 cols of cur row
        vaddpd ymm1,ymm0,ymmword ptr [edi]  ;add to col_means
        vmovapd ymmword ptr [edi],ymm1      ;save updated col_means
        add ecx,4                           ;col_index += 4
        add esi,32                          ;update x ptr
        add edi,32                          ;update col_means ptr
        jmp NextColSet

@@:     sub eax,2
        cmp eax,ebx                         ;2 or more columns remaining?
        jg @F                               ;jump if col_index + 2 > ncols

; Update col_means using next two columns
        vmovupd xmm0,xmmword ptr [esi]      ;load next 2 cols of cur row
        vaddpd xmm1,xmm0,xmmword ptr [edi]  ;add to col_meanss
        vmovapd xmmword ptr [edi],xmm1      ;save updated col_meanss
        add ecx,2                           ;col_index += 2
        add esi,16                          ;update x ptr
        add edi,16                          ;update col_means ptr
        jmp NextColSet

; Update col_means using next column (or last column in the current row)
@@:     vmovsd xmm0,real8 ptr [esi]         ;load x from last column
        vaddsd xmm1,xmm0,real8 ptr [edi]    ;add to col_means
        vmovsd real8 ptr [edi],xmm1         ;save updated col_means
        add ecx,1                           ;col_index += 1
        add esi,8                           ;update x ptr

NextColSet:
        cmp ecx,ebx                         ;more columns in current row?
        jl LP2                              ;jump if yes
        dec edx                             ;nrows -= 1
        jnz LP1                             ;jump if more rows

; Compute the final col_means
        mov eax,[ebp+12]                    ;eax = nrows
        vcvtsi2sd xmm2,xmm2,eax             ;xmm2 = DPFP nrows
        mov edx,[ebp+16]                    ;edx = ncols
        mov edi,[ebp+20]                    ;edi = ptr to col_means

@@:     vmovsd xmm0,real8 ptr [edi]         ;xmm0 = col_means[i]
        vdivsd xmm1,xmm0,xmm2               ;compute final mean
        vmovsd real8 ptr [edi],xmm1         ;save col_mean[i]
        add edi,8                           ;update col_means ptr
        dec edx                             ;ncols -= 1
        jnz @B                              ;repeat until done
        mov eax,1                           ;set success return code
        vzeroupper

BadArg: pop edi
        pop esi
        pop ebx
        pop ebp
        ret

AvxPfpColMeans_ endp
        end
