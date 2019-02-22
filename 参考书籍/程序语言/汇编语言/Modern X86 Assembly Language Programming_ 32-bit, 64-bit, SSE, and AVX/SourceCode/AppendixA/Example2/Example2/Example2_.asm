        .model flat,c
        .code

; extern "C" void CalcResult2_(int a, int b, int c, int* quo, int* rem);

CalcResult2_ proc
        push ebp
        mov ebp,esp

; Calculate (a + b) / c
        mov eax,[ebp+8]                     ;eax = a
        mov ecx,[ebp+12]                    ;ecx = b
        add eax,ecx                         ;eax = a + b

        cdq                                 ;edx:eax contains dividend
        idiv dword ptr [ebp+16]             ;eax = quotient, edx = rem

        mov ecx,[ebp+20]                    ;ecx = ptr to quo
        mov dword ptr [ecx],eax             ;save quotent
        mov ecx,[ebp+24]                    ;ecx = ptr to rem
        mov dword ptr [ecx],edx             ;save remainder

        pop ebp
        ret
CalcResult2_ endp
        end
