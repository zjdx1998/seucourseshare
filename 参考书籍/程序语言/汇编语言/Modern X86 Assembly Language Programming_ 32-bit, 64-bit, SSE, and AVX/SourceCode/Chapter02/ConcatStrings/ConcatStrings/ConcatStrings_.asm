        .model flat,c
        .code

; extern "C" int ConcatStrings_(wchar_t* des, int des_size, const wchar_t* const* src, int src_n)
;
; Description:  This function performs string concatenation using
;               multiple input strings.
;
; Returns:      -1          Invalid 'des_size'
;               n >= 0      Length of concatenated string
;
; Locals Vars:  [ebp-4] = des_index
;               [ebp-8] = i

ConcatStrings_ proc
        push ebp
        mov ebp,esp
        sub esp,8
        push ebx
        push esi
        push edi

; Make sure 'des_size' is valid
        mov eax,-1
        mov ecx,[ebp+12]                    ;ecx = 'des_size'
        cmp ecx,0
        jle Error

; Perform required initializations
        xor eax,eax
        mov ebx,[ebp+8]                     ;ebx = 'des'
        mov [ebx],ax                        ;*des = '\0'
        mov [ebp-4],eax                     ;des_index = 0
        mov [ebp-8],eax                     ;i = 0

; Repeat loop until concatenation is finished
Lp1:    mov eax,[ebp+16]                    ;eax = 'src'
        mov edx,[ebp-8]                     ;edx = i
        mov edi,[eax+edx*4]                 ;edi = src[i]
        mov esi,edi                         ;esi = src[i]

; Compute length of s[i]
        xor eax,eax
        mov ecx,-1
        repne scasw                         ;find '\0'
        not ecx
        dec ecx                             ;ecx = len(src[i])

; Compute des_index + src_len
        mov eax,[ebp-4]                     ;eax= des_index
        mov edx,eax                         ;edx = des_index_temp
        add eax,ecx                         ;des_index + len(src[i])

; Is des_index + src_len >= des_size?
        cmp eax,[ebp+12]
        jge Done

; Update des_index
        add [ebp-4],ecx                     ;des_index += len(src[i])

; Copy src[i] to &des[des_index] (esi already contains src[i])
        inc ecx                             ;ecx = len(src[i]) + 1
        lea edi,[ebx+edx*2]                 ;edi = &des[des_index_temp]
        rep movsw                           ;perform string move

; Update i and repeat if not done
        mov eax,[ebp-8]
        inc eax
        mov [ebp-8],eax                     ;i++
        cmp eax,[ebp+20]
        jl Lp1                              ;jump if i < src_n

; Return length of concatenated string
Done:   mov eax,[ebp-4]                     ;eax = des_index
Error:  pop edi
        pop esi
        pop ebx
        mov esp,ebp
        pop ebp
        ret
ConcatStrings_ endp
        end
