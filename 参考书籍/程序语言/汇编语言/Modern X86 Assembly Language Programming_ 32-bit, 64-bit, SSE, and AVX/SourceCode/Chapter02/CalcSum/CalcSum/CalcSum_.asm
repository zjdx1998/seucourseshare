    .model flat,c
    .code

; extern "C" int CalcSum_(int a, int b, int c)
;
; Description:  This function demonstrates passing arguments between
;               a C++ function and an assembly language function.
;
; Returns:      a + b + c

CalcSum_ proc

; Initialize a stack frame pointer
        push ebp
        mov ebp,esp

; Load the argument values
        mov eax,[ebp+8]                     ; eax = 'a'
        mov ecx,[ebp+12]                    ; ecx = 'b'
        mov edx,[ebp+16]                    ; edx = 'c'

; Calculate the sum
        add eax,ecx                         ; eax = 'a' + 'b'
        add eax,edx                         ; eax = 'a' + 'b' + 'c'

; Restore the caller's stack frame pointer
        pop ebp
        ret

CalcSum_ endp
        end
