        .model flat,c
        .code

; extern "C" int CountChars_(wchar_t* s, wchar_t c);
;
; Description:  This function counts the number of occurrences
;               of 'c' in 's'
;
; Returns:      Number of occurrences of 'c'

CountChars_ proc
        push ebp
        mov ebp,esp
        push esi

; Load parameters and initialize count registers
        mov esi,[ebp+8]                     ;esi = 's'
        mov cx,[ebp+12]                     ;cx = 'c'
        xor edx,edx                         ;edx = Number of occurrences

; Repeat loop until the entire string has been scanned
@@:     lodsw                               ;load next char into ax
        or ax,ax                            ;test for end-of-string
        jz @F                               ;jump if end-of-string found
        cmp ax,cx                           ;test current char
        jne @B                              ;jump if no match
        inc edx                             ;update match count
        jmp @B

@@:     mov eax,edx                         ;eax = character count
        pop esi
        pop ebp
        ret
CountChars_ endp
        end
