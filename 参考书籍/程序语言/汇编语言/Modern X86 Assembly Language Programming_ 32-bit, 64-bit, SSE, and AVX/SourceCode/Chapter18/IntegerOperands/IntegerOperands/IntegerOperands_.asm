; The following structure must match the structure that's
; declared in IntegerOperands.cpp. Note the version below
; includes "pad" bytes, which are needed to account for the
; member alignments performed by the C++ compiler.
ClVal   struct
a8      byte ?
pad1    byte ?
a16     word ?
a32     dword ?
a64     qword ?
b8      byte ?
pad2    byte ?
b16     word ?
b32     dword ?
b64     qword ?
ClVal   ends

        .code

; extern "C" void CalcLogical_(ClVal* cl_val, Uint8 c8[3], Uint16 c16[3], Uint32 c32[3], Uint64 c64[3]);
;
; Description:  The following function demonstrates logical operations
;               using different sizes of integers.

CalcLogical_ proc

; 8-bit logical operations
        mov r10b,[rcx+ClVal.a8]             ;r10b = a8
        mov r11b,[rcx+ClVal.b8]             ;r11b = b8
        mov al,r10b
        and al,r11b                         ;calc a8 & b8
        mov [rdx],al
        mov al,r10b
        or al,r11b                          ;calc a8 | b8
        mov [rdx+1],al
        mov al,r10b
        xor al,r11b                         ;calc a8 ^ b8
        mov [rdx+2],al

; 16-bit logical operations
        mov rdx,r8                          ;rdx = ptr to c16
        mov r10w,[rcx+ClVal.a16]            ;r10w = a16
        mov r11w,[rcx+ClVal.b16]            ;r11w = b16
        mov ax,r10w
        and ax,r11w                         ;calc a16 & b16
        mov [rdx],ax
        mov ax,r10w
        or ax,r11w                          ;calc a16 | b16
        mov [rdx+2],ax
        mov ax,r10w
        xor ax,r11w                         ;calc a16 ^ b16
        mov [rdx+4],ax

; 32-bit logical operations
        mov rdx,r9                          ;rdx = ptr to c32
        mov r10d,[rcx+ClVal.a32]            ;r10d = a32
        mov r11d,[rcx+ClVal.b32]            ;r11d = b32
        mov eax,r10d
        and eax,r11d                        ;calc a32 & b32
        mov [rdx],eax
        mov eax,r10d
        or eax,r11d                         ;calc a32 | b32
        mov [rdx+4],eax
        mov eax,r10d
        xor eax,r11d                        ;calc a32 ^ b32
        mov [rdx+8],eax

; 64-bit logical operations
        mov rdx,[rsp+40]                    ;rdx = ptr to c64
        mov r10,[rcx+ClVal.a64]             ;r10 = a64
        mov r11,[rcx+ClVal.b64]             ;r11 = b64
        mov rax,r10
        and rax,r11                         ;calc a64 & b64
        mov [rdx],rax
        mov rax,r10
        or rax,r11                          ;calc a64 | b64
        mov [rdx+8],rax
        mov rax,r10
        xor rax,r11                         ;calc a64 ^ b64
        mov [rdx+16],rax

        ret
CalcLogical_ endp
        end
