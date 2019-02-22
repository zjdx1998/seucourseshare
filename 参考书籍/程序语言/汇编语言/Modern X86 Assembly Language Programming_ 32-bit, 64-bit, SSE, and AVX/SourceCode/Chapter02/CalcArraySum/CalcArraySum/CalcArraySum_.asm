        .model flat,c
        .code

; extern "C" int CalcArraySum_(const int* x, int n);
;
; Description:  This function sums the elements of a signed
;               integer array.

CalcArraySum_ proc
        push ebp
        mov ebp,esp

; Load arguments and initialize sum
        mov edx,[ebp+8]                     ;edx = 'x'
        mov ecx,[ebp+12]                    ;ecx = 'n'
        xor eax,eax                         ;eax = sum

; Make sure 'n' is greater than zero
        cmp ecx,0
        jle InvalidCount

; Calculate the array element sum
@@:     add eax,[edx]                       ;add next element to sum
        add edx,4                           ;set pointer to next element
        dec ecx                             ;adjust counter
        jnz @B                              ;repeat if not done

InvalidCount:
        pop ebp
        ret
CalcArraySum_ endp
        end
