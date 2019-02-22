        .model flat,c
        .code

; extern "C" int CalcArraySquares_(int* y, const int* x, int n);
;
;Description:   This function cComputes y[i] = x[i] * x[i].
;
; Returns:      Sum of the elements in array y.

CalcArraySquares_ proc
        push ebp
        mov ebp,esp
        push ebx
        push esi
        push edi

; Load arguments
        mov edi,[ebp+8]                     ;edi = 'y'
        mov esi,[ebp+12]                    ;esi = 'x'
        mov ecx,[ebp+16]                    ;ecx = 'n'

; Initialize array sum register, calculate size of array in bytes,
; and initialize element offset register.
        xor eax,eax                         ;eax = sum of 'y' array
        cmp ecx,0
        jle EmptyArray
        shl ecx,2                           ;ecx = size of array in bytes
        xor ebx,ebx                         ;ebx = array element offset

; Repeat loop until finished
@@:     mov edx,[esi+ebx]                   ;load next x[i]
        imul edx,edx                        ;compute x[i] * x[i]
        mov [edi+ebx],edx                   ;save result to y[i]
        add eax,edx                         ;update running sum
        add ebx,4                           ;update array element offset
        cmp ebx,ecx
        jl @B                               ;jump if not finished

EmptyArray:
        pop edi
        pop esi
        pop ebx
        pop ebp
        ret
CalcArraySquares_ endp
        end
