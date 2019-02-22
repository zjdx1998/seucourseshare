        .model flat,c
        .code

; extern "C" void ReverseArray_(int* y, const int* x, int n);
;
; Description:  The following function saves the elements of array 'x'
;               to array 'y' in reverse order.
;
; Returns       0 = Invalid 'n'
;               1 = Success

ReverseArray_ proc
        push ebp
        mov ebp,esp
        push esi
        push edi

; Load arguments, make sure 'n' is valid
        xor eax,eax                         ;error return code
        mov edi,[ebp+8]                     ;edi = 'y'
        mov esi,[ebp+12]                    ;esi = 'x'
        mov ecx,[ebp+16]                    ;ecx = 'n'
        test ecx,ecx
        jle Error                           ;jump if 'n' <= 0

; Initialize pointer to x[n - 1] and direction flag
        lea esi,[esi+ecx*4-4]               ;esi = &x[n - 1]
        pushfd                              ;save current direction flag
        std                                 ;EFLAGS.DF = 1

; Repeat loop until array reversal is complete
@@:     lodsd                               ;eax = *x--
        mov [edi],eax                       ;*y = eax
        add edi,4                           ;y++
        dec ecx                             ;n--
        jnz @B

        popfd                               ;restore direction flag
        mov eax,1                           ;set success return code

Error:  pop edi
        pop esi
        pop ebp
        ret
ReverseArray_ endp
        end
