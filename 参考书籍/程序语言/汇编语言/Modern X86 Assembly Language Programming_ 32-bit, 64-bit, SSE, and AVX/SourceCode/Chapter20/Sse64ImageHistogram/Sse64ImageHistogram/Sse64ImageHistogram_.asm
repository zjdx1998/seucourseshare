        include <MacrosX86-64.inc>
        .code
        extern NUM_PIXELS_MAX:dword

; extern bool Sse64ImageHistogram_(Uint32* histo, const Uint8* pixel_buff, Uint32 num_pixels);
;
; Description:  The following function builds an image histogram.
;
; Returns:      0 = invalid argument value
;               1 = success
;
; Requires:     X86-64, SSE4.1

Sse64ImageHistogram_ proc frame
        _CreateFrame Sse64Ih_,1024,0,rbx,rsi,rdi
        _EndProlog

; Make sure num_pixels is valid
        test r8d,r8d
        jz Error                            ;jump if num_pixels is zero
        cmp r8d,[NUM_PIXELS_MAX]
        ja Error                            ;jump if num_pixels too big
        test r8d,1fh
        jnz Error                           ;jump if num_pixels % 32 != 0

; Make sure histo & pixel_buff are properly aligned
        mov rsi,rcx                         ;rsi = ptr to histo
        test rsi,0fh
        jnz Error                           ;jump if histo misaligned
        mov r9,rdx
        test r9,0fh
        jnz Error                           ;jump if pixel_buff misaligned

; Initialize local histogram buffers (set all entries to zero)
        xor rax,rax
        mov rdi,rsi                         ;rdi = ptr to histo
        mov rcx,128                         ;rcx = size in qwords
        rep stosq                           ;zero histo
        mov rdi,rbp                         ;rdi = ptr to histo2
        mov rcx,128                         ;rcx = size in qwords
        rep stosq                           ;zero histo2

; Perform processing loop initializations
        shr r8d,5                           ;r8d = number of pixel blocks
        mov rdi,rbp                         ;rdi = ptr to histo2

; Build the histograms
        align 16                            ;align jump target
@@:     movdqa xmm0,[r9]                    ;load pixel block
        movdqa xmm2,[r9+16]                 ;load pixel block
        movdqa xmm1,xmm0
        movdqa xmm3,xmm2

; Process pixels 0 - 3
        pextrb rax,xmm0,0
        add dword ptr [rsi+rax*4],1         ;count pixel 0
        pextrb rbx,xmm1,1
        add dword ptr [rdi+rbx*4],1         ;count pixel 1
        pextrb rcx,xmm0,2
        add dword ptr [rsi+rcx*4],1         ;count pixel 2
        pextrb rdx,xmm1,3
        add dword ptr [rdi+rdx*4],1         ;count pixel 3

; Process pixels 4 - 7
        pextrb rax,xmm0,4
        add dword ptr [rsi+rax*4],1         ;count pixel 4
        pextrb rbx,xmm1,5
        add dword ptr [rdi+rbx*4],1         ;count pixel 5
        pextrb rcx,xmm0,6
        add dword ptr [rsi+rcx*4],1         ;count pixel 6
        pextrb rdx,xmm1,7
        add dword ptr [rdi+rdx*4],1         ;count pixel 7

; Process pixels 8 - 11
        pextrb rax,xmm0,8
        add dword ptr [rsi+rax*4],1         ;count pixel 8
        pextrb rbx,xmm1,9
        add dword ptr [rdi+rbx*4],1         ;count pixel 9
        pextrb rcx,xmm0,10
        add dword ptr [rsi+rcx*4],1         ;count pixel 10
        pextrb rdx,xmm1,11
        add dword ptr [rdi+rdx*4],1         ;count pixel 11

; Process pixels 12 - 15
        pextrb rax,xmm0,12
        add dword ptr [rsi+rax*4],1         ;count pixel 12
        pextrb rbx,xmm1,13
        add dword ptr [rdi+rbx*4],1         ;count pixel 13
        pextrb rcx,xmm0,14
        add dword ptr [rsi+rcx*4],1         ;count pixel 14
        pextrb rdx,xmm1,15
        add dword ptr [rdi+rdx*4],1         ;count pixel 15

; Process pixels 16 - 19
        pextrb rax,xmm2,0
        add dword ptr [rsi+rax*4],1         ;count pixel 16
        pextrb rbx,xmm3,1
        add dword ptr [rdi+rbx*4],1         ;count pixel 17
        pextrb rcx,xmm2,2
        add dword ptr [rsi+rcx*4],1         ;count pixel 18
        pextrb rdx,xmm3,3
        add dword ptr [rdi+rdx*4],1         ;count pixel 19

; Process pixels 20 - 23
        pextrb rax,xmm2,4
        add dword ptr [rsi+rax*4],1         ;count pixel 20
        pextrb rbx,xmm3,5
        add dword ptr [rdi+rbx*4],1         ;count pixel 21
        pextrb rcx,xmm2,6
        add dword ptr [rsi+rcx*4],1         ;count pixel 22
        pextrb rdx,xmm3,7
        add dword ptr [rdi+rdx*4],1         ;count pixel 23

; Process pixels 24 - 27
        pextrb rax,xmm2,8
        add dword ptr [rsi+rax*4],1         ;count pixel 24
        pextrb rbx,xmm3,9
        add dword ptr [rdi+rbx*4],1         ;count pixel 25
        pextrb rcx,xmm2,10
        add dword ptr [rsi+rcx*4],1         ;count pixel 26
        pextrb rdx,xmm3,11
        add dword ptr [rdi+rdx*4],1         ;count pixel 27

; Process pixels 28 - 31
        pextrb rax,xmm2,12
        add dword ptr [rsi+rax*4],1         ;count pixel 28
        pextrb rbx,xmm3,13
        add dword ptr [rdi+rbx*4],1         ;count pixel 29
        pextrb rcx,xmm2,14
        add dword ptr [rsi+rcx*4],1         ;count pixel 30
        pextrb rdx,xmm3,15
        add dword ptr [rdi+rdx*4],1         ;count pixel 31

        add r9,32                           ;r9  = next pixel block
        sub r8d,1
        jnz @B                              ;repeat loop if not done

; Merge intermediate histograms into final histogram
        mov ecx,32                          ;ecx = num iterations
        xor rax,rax                         ;rax = common offset

@@:     movdqa xmm0,xmmword ptr [rsi+rax]       ;load histo counts
        movdqa xmm1,xmmword ptr [rsi+rax+16]
        paddd xmm0,xmmword ptr [rdi+rax]        ;add counts from histo2
        paddd xmm1,xmmword ptr [rdi+rax+16]
        movdqa xmmword ptr [rsi+rax],xmm0       ;save final result
        movdqa xmmword ptr [rsi+rax+16],xmm1

        add rax,32
        sub ecx,1
        jnz @B
        mov eax,1                           ;set success return code

Done:  _DeleteFrame rbx,rsi,rdi
        ret

Error:  xor eax,eax                         ;set error return code
        jmp Done
Sse64ImageHistogram_ endp
        end
