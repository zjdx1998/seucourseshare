        .model flat,c
        .code

; extern "C" int CalcResult1_(int a, int b, int c);

CalcResult1_ proc
        push ebp
        mov ebp,esp

        mov eax,[ebp+8]                     ;eax = a
        mov ecx,[ebp+12]                    ;ecx = b
        mov edx,[ebp+16]                    ;edx = c

        add eax,ecx                         ;eax = a + b
        imul eax,edx                        ;eax = (a + b) * c

        pop ebp
        ret
CalcResult1_ endp
        end
