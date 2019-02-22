        .model flat,c
        .code

; extern "C" int IntegerMulDiv_(int a, int b, int* prod, int* quo, int* rem);
;
; Description:  This function demonstrates use of the imul and idiv
;               instructions.  It also illustrates pointer usage.
;
; Returns:      0   Error (divisor is zero)
;               1   Success (divisor is zero)
;
; Computes:     *prod = a * b;
;               *quo = a / b
;               *rem = a % b

IntegerMulDiv_ proc

; Function prolog
        push ebp
        mov ebp,esp
        push ebx

; Make sure the divisor is not zero
        xor eax,eax                         ;set error return code
        mov ecx,[ebp+8]                     ;ecx = 'a'
        mov edx,[ebp+12]                    ;edx = 'b'
        or edx,edx
        jz InvalidDivisor                   ;jump if 'b' is zero

; Calculate product and save result
        imul edx,ecx                        ;edx = 'a' * 'b'
        mov ebx,[ebp+16]                    ;ebx = 'prod'
        mov [ebx],edx                       ;save product

; Calculate quotient and remainder, save results
        mov eax,ecx                         ;eax = 'a'
        cdq                                 ;edx:eax contains dividend
        idiv dword ptr [ebp+12]             ;eax = quo, edx = rem

        mov ebx,[ebp+20]                    ;ebx = 'quo'
        mov [ebx],eax                       ;save quotient
        mov ebx,[ebp+24]                    ;ebx = 'rem'
        mov [ebx],edx                       ;save remainder
        mov eax,1                           ;set success return code

; Function epilog
InvalidDivisor:
        pop ebx
        pop ebp
        ret
IntegerMulDiv_ endp
        end
