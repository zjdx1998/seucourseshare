        .model flat,c

; These are defined in IntegerAddition.cpp
        extern GlChar:byte
        extern GlShort:word
        extern GlInt:dword
        extern GlLongLong:qword

; extern "C" void IntegerTypes_(char a, short b, int c, long long d);
;
; Description:  This function demonstrates simple addition using
;               various-sized integers.
;
; Returns:  None.

        .code
IntegerAddition_ proc

; Function prolog
        push ebp
        mov ebp,esp

; Compute GlChar += a
        mov al,[ebp+8]
        add [GlChar],al

; Compute GlShort += b, note offset of 'b' on stack
        mov ax,[ebp+12]
        add [GlShort],ax

; Compute GlInt += c, note offset of 'c' on stack
        mov eax,[ebp+16]
        add [GlInt],eax

; Compute GlLongLong += d, note use of dword ptr operator and adc
        mov eax,[ebp+20]
        mov edx,[ebp+24]
        add dword ptr [GlLongLong],eax
        adc dword ptr [GlLongLong+4],edx

; Function epilog
        pop ebp
        ret
IntegerAddition_ endp
        end
