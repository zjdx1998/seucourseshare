        .model flat,c
        .code

; extern "C" void CalculateSums_(int a, int b, int c, int* s1, int* s2, int* s3);
;
; Description:  This function demonstrates a complete assembly language
;               prolog and epilog.
;
; Returns:  None.
;
; Computes:     *s1 = a + b + c
;               *s2 = a * a + b * b + c * c
;               *s3 = a * a * a + b * b * b + c * c * c

CalculateSums_  proc

; Function prolog
        push ebp
        mov ebp,esp
        sub esp,12                          ;Allocate local storage space
        push ebx
        push esi
        push edi

; Load arguments
        mov eax,[ebp+8]                     ;eax = 'a'
        mov ebx,[ebp+12]                    ;ebx = 'b'
        mov ecx,[ebp+16]                    ;ecx = 'c'
        mov edx,[ebp+20]                    ;edx = 's1'
        mov esi,[ebp+24]                    ;esi = 's2'
        mov edi,[ebp+28]                    ;edi = 's3'

; Compute 's1'
        mov [ebp-12],eax
        add [ebp-12],ebx
        add [ebp-12],ecx                    ;final 's1' result

; Compute 's2'
        imul eax,eax
        imul ebx,ebx
        imul ecx,ecx
        mov [ebp-8],eax
        add [ebp-8],ebx
        add [ebp-8],ecx                     ;final 's2' result

; Compute 's3'
        imul eax,[ebp+8]
        imul ebx,[ebp+12]
        imul ecx,[ebp+16]
        mov [ebp-4],eax
        add [ebp-4],ebx
        add [ebp-4],ecx                     ;final 's3' result

; Save 's1', 's2', and 's3'
        mov eax,[ebp-12]
        mov [edx],eax                       ;save 's1'
        mov eax,[ebp-8]
        mov [esi],eax                       ;save 's2'
        mov eax,[ebp-4]
        mov [edi],eax                       ;save 's3'

; Function epilog
        pop edi
        pop esi
        pop ebx
        mov esp,ebp                         ;Release local storage space
        pop ebp
        ret
CalculateSums_  endp
        end
