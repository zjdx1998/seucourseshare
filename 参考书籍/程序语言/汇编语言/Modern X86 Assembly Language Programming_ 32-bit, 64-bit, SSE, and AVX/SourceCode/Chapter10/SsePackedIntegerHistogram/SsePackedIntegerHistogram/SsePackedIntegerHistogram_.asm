        .model flat,c
        .code
        extern NUM_PIXELS_MAX:dword

; extern bool SsePiHistogram_(Uint32* histo, const Uint8* pixel_buff, Uint32 num_pixels);
;
; Description:  The following function builds an image histogram.
;
; Returns:      0 = invalid argument value
;               1 = success
;
; Requires:     SSE4.1

SsePiHistogram_ proc
        push ebp
        mov ebp,esp
        and esp,0FFFFFFF0H                  ;align ESP to 16 byte boundary
        sub esp,1024                        ;allocate histo2
        mov edx,esp                         ;edx = histo2
        push ebx
        push esi
        push edi

; Make sure num_pixels is valid
        xor eax,eax                         ;set error return code
        mov ecx,[ebp+16]                    ;ecx = num_pixels
        cmp ecx,[NUM_PIXELS_MAX]
        ja Done                             ;jump if num_pixels too big
        test ecx,1fh
        jnz Done                            ;jump if num_pixels % 32 != 0

; Make sure histo & pixel_buff are properly aligned
        mov ebx,[ebp+8]                     ;ebx = histo
        test ebx,0fh
        jnz Done                            ;jump if misaligned
        mov esi,[ebp+12]                    ;esi = pixel_buff
        test esi,0fh
        jnz Done                            ;jump if misaligned

; Initialize the histogram buffers (set all entries to zero)
        mov edi,ebx                         ;edi = histo
        mov ecx,256
        rep stosd                           ;initialize histo
        mov edi,edx                         ;edi = histo2
        mov ecx,256
        rep stosd                           ;initialize histo2

; Perform processing loop initializations
        mov edi,edx                         ;edi = histo2
        mov ecx,[ebp+16]                    ;ecx = number of pxiels
        shr ecx,5                           ;ecx = number of pixel blocks

; Build the histograms
; Register usage: ebx = histo, edi = histo2, esi = pixel_buff
        align 16                            ;align jump target
@@:     movdqa xmm0,[esi]                   ;load pixel block
        movdqa xmm2,[esi+16]                ;load pixel block
        movdqa xmm1,xmm0
        movdqa xmm3,xmm2

; Process pixels 0 - 3
        pextrb eax,xmm0,0                   ;extract & count pixel 0
        add dword ptr [ebx+eax*4],1
        pextrb edx,xmm1,1                   ;extract & count pixel 1
        add dword ptr [edi+edx*4],1
        pextrb eax,xmm0,2                   ;extract & count pixel 2
        add dword ptr [ebx+eax*4],1
        pextrb edx,xmm1,3                   ;extract & count pixel 3
        add dword ptr [edi+edx*4],1

; Process pixels 4 - 7
        pextrb eax,xmm0,4                   ;extract & count pixel 4
        add dword ptr [ebx+eax*4],1
        pextrb edx,xmm1,5                   ;extract & count pixel 5
        add dword ptr [edi+edx*4],1
        pextrb eax,xmm0,6                   ;extract & count pixel 6
        add dword ptr [ebx+eax*4],1
        pextrb edx,xmm1,7                   ;extract & count pixel 7
        add dword ptr [edi+edx*4],1

; Process pixels 8 - 11
        pextrb eax,xmm0,8                   ;extract & count pixel 8
        add dword ptr [ebx+eax*4],1
        pextrb edx,xmm1,9                   ;extract & count pixel 9
        add dword ptr [edi+edx*4],1
        pextrb eax,xmm0,10                  ;extract & count pixel 10
        add dword ptr [ebx+eax*4],1
        pextrb edx,xmm1,11                  ;extract & count pixel 11
        add dword ptr [edi+edx*4],1

; Process pixels 12 - 15
        pextrb eax,xmm0,12                  ;extract & count pixel 12
        add dword ptr [ebx+eax*4],1
        pextrb edx,xmm1,13                  ;extract & count pixel 13
        add dword ptr [edi+edx*4],1
        pextrb eax,xmm0,14                  ;extract & count pixel 14
        add dword ptr [ebx+eax*4],1
        pextrb edx,xmm1,15                  ;extract & count pixel 15
        add dword ptr [edi+edx*4],1

; Process pixels 16 - 19
        pextrb eax,xmm2,0                   ;extract & count pixel 16
        add dword ptr [ebx+eax*4],1
        pextrb edx,xmm3,1                   ;extract & count pixel 17
        add dword ptr [edi+edx*4],1
        pextrb eax,xmm2,2                   ;extract & count pixel 18
        add dword ptr [ebx+eax*4],1
        pextrb edx,xmm3,3                   ;extract & count pixel 19
        add dword ptr [edi+edx*4],1

; Process pixels 20 - 23
        pextrb eax,xmm2,4                   ;extract & count pixel 20
        add dword ptr [ebx+eax*4],1
        pextrb edx,xmm3,5                   ;extract & count pixel 21
        add dword ptr [edi+edx*4],1
        pextrb eax,xmm2,6                   ;extract & count pixel 22
        add dword ptr [ebx+eax*4],1
        pextrb edx,xmm3,7                   ;extract & count pixel 23
        add dword ptr [edi+edx*4],1

; Process pixels 24 - 27
        pextrb eax,xmm2,8                   ;extract & count pixel 24
        add dword ptr [ebx+eax*4],1
        pextrb edx,xmm3,9                   ;extract & count pixel 25
        add dword ptr [edi+edx*4],1
        pextrb eax,xmm2,10                  ;extract & count pixel 26
        add dword ptr [ebx+eax*4],1
        pextrb edx,xmm3,11                  ;extract & count pixel 27
        add dword ptr [edi+edx*4],1

; Process pixels 28 - 31
        pextrb eax,xmm2,12                  ;extract & count pixel 28
        add dword ptr [ebx+eax*4],1
        pextrb edx,xmm3,13                  ;extract & count pixel 29
        add dword ptr [edi+edx*4],1
        pextrb eax,xmm2,14                  ;extract & count pixel 30
        add dword ptr [ebx+eax*4],1
        pextrb edx,xmm3,15                  ;extract & count pixel 31
        add dword ptr [edi+edx*4],1

        add esi,32                          ;esi = ptr next pixel block
        sub ecx,1                           ;update counter
        jnz @B                              ;repeat loop if not done

; Add histo2 to histo for final histogram. Note that each loop iteration
; adds 8 histogram entries.
        mov ecx,32                          ;ecx = number of iterations
        xor eax,eax                         ;eax = offset for histo arrays

@@:     movdqa xmm0,xmmword ptr [ebx+eax]       ;load histo counts
        movdqa xmm1,xmmword ptr [ebx+eax+16]

        paddd xmm0,xmmword ptr [edi+eax]        ;add counts from histo2
        paddd xmm1,xmmword ptr [edi+eax+16]

        movdqa xmmword ptr [ebx+eax],xmm0       ;save final histo counts
        movdqa xmmword ptr [ebx+eax+16],xmm1

        add eax,32                          ;update array offset
        sub ecx,1                           ;update counter
        jnz @B                              ;repeat loop if not done
        mov eax,1                           ;set success return code

Done:   pop edi
        pop esi
        pop ebx
        mov esp,ebp
        pop ebp
        ret
SsePiHistogram_ endp
        end
