        include <MacrosX86-64.inc>
        .code

; extern "C" int ConcatStrings_(wchar_t* des, int des_size, const wchar_t* const* src, int src_n)
;
; Description:  This function performs string concatenation using
;               multiple input strings.
;
; Returns:      -1          Invalid des_size or src_n
;               n >= 0      Length of concatenated string

ConcatStrings_ proc frame
        _CreateFrame ConcatStrings_,0,0,rbx,rsi,rdi
        _EndProlog

; Make sure des_size and src_n are  greater than zero
        movsxd rdx,edx                      ;rdx = des_size
        test rdx,rdx
        jle Error                           ;jump if des_size <= 0
        movsxd r9,r9d                       ;r9 = src_n
        test r9,r9
        jle Error                           ;jump if src_n <= 0

; Perform required initializations
        mov rbx,rcx                         ;rbx = des
        xor r10,r10                         ;des_index = 0
        xor r11,r11                         ;i = 0
        mov word ptr [rbx],r10w             ;*des = '\0';

; Repeat loop until concatenation is finished
Lp1:    mov rdi,[r8+r11*8]                  ;rdi = src[i]
        mov rsi,rdi                         ;rsi = src[i]

; Compute length of s[i]
        xor rax,rax
        mov rcx,-1
        repne scasw                         ;find '\0'
        not rcx
        dec rcx                             ;rcx = len(src[i])

; Compute des_index + src_len
        mov rax,r10                         ;rax= des_index
        add rax,rcx                         ;rax = des_index + len(src[i])

; Is des_index + src_len >= des_size?
        cmp rax,rdx
        jge Done

; Copy src[i] to &des[des_index] (rsi already contains src[i])
        inc rcx                             ;rcx = len(src[i]) + 1
        lea rdi,[rbx+r10*2]                 ;rdi = &des[des_index]
        rep movsw                           ;perform string move

; Update des_index
        mov r10,rax                         ;des_index += len(src[i])

; Update i and repeat if not done
        inc r11                             ;i += 1
        cmp r11,r9                          ;is i >= src_n?
        jl Lp1                              ;jump if i < src_n

; Return length of concatenated string
Done:   mov eax,r10d                        ;eax = trunc(des_index)
       _DeleteFrame rbx,rsi,rdi
        ret

; Return error code
Error:  mov eax,-1                          ;eax = error code
       _DeleteFrame rbx,rsi,rdi
        ret

ConcatStrings_ endp
        end
