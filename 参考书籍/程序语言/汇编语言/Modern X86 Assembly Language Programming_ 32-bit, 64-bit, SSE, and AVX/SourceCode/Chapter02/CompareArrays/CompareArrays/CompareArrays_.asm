        .model flat,c
        .code

; extern "C" int CompareArrays_(const int* x, const int* y, int n)
;
; Description:  This function compares two integer arrays element
;               by element for equality
;
; Returns       -1          Value of 'n' is invalid
;               0 <= i < n  Index of first non-matching element
;               n           All elements match

CompareArrays_ proc
        push ebp
        mov ebp,esp
        push esi
        push edi

; Load arguments and validate 'n'
        mov eax,-1                          ;invalid 'n' return code
        mov esi,[ebp+8]                     ;esi = 'x'
        mov edi,[ebp+12]                    ;edi = 'y'
        mov ecx,[ebp+16]                    ;ecx = 'n'
        test ecx,ecx
        jle @F                              ;jump if 'n' <= 0
        mov eax,ecx                         ;eax = 'n

; Compare the arrays for equality
        repe cmpsd
        je @F                               ;arrays are equal

; Calculate index of unequal elements
        sub eax,ecx
        dec eax                             ;eax = index of mismatch

@@:     pop edi
        pop esi
        pop ebp
        ret
CompareArrays_ endp
        end
