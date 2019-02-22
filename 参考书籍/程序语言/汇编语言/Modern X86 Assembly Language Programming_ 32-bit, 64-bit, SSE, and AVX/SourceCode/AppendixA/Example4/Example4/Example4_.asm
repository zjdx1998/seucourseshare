; extern "C" bool CalcResult4_(int* y, const int* x, int n);

IFDEF ASMX86_32
        .model flat,c
        .code

CalcResult4_ proc
        push ebp
        mov ebp,esp
        push ebx
        push esi

        mov ecx,[ebp+8]                     ;ecx = ptr to y
        mov edx,[ebp+12]                    ;edx = ptr to x
        mov ebx,[ebp+16]                    ;eax = n
        test ebx,ebx                        ;is n <= 0?
        jle Error                           ;jump if n <= 0

        xor esi,esi                         ;i = 0;
@@:     mov eax,[edx+esi*4]                 ;eax = x[i]
        imul eax,eax                        ;eax = x[i] * x[i]
        mov [ecx+esi*4],eax                 ;save result to y[i]

        add esi,1                           ;i = i + 1
        cmp esi,ebx
        jl @B                               ;jump if i < n

        mov eax,1                           ;set success return code
        pop esi
        pop ebx
        pop ebp
        ret

Error:  xor eax,eax                         ;set error return code
        pop esi
        pop ebx
        pop ebp
        ret

CalcResult4_ endp
ENDIF

IFDEF ASMX86_64
        .code

CalcResult4_ proc

; Register arguments: rcx = ptr to y, rdx = ptr to x, and r8d = n
        movsxd r8,r8d                       ;sign-extend n to 64 bits
        test r8,r8                          ;is n <= 0?
        jle Error                           ;jump if n <= 0

        xor r9,r9                           ;i = 0;
@@:     mov eax,[rdx+r9*4]                  ;eax = x[i]
        imul eax,eax                        ;eax = x[i] * x[i]
        mov [rcx+r9*4],eax                  ;save result to y[i]

        add r9,1                            ;i = i + 1
        cmp r9,r8
        jl @B                               ;jump if i < n

        mov eax,1                           ;set success return code
        ret

Error:  xor eax,eax                         ;set error return code
        ret

CalcResult4_ endp
ENDIF
        end
