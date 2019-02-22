        .model flat,c
        .code

; extern "C" int SseTextStringCalcLength_(const char* s);
;
; Description:  The following function calculates the length of a
;               text string using the x86-SSE instruction pcmpistri.
;
; Returns:      Length of text string
;
; Requires      SSE4.2

SseTextStringCalcLength_ proc
        push ebp
        mov ebp,esp

; Initialize registers for string length calculation
        mov eax,[ebp+8]                     ;eax ='s'
        sub eax,16                          ;adjust eax for use in loop
        mov edx,0ff01h
        movd xmm1,edx                       ;xmm1[15:0] = char range

; Calculate next address and test for near end-of-page condition
@@:     add eax,16              ;eax = next text block
        mov edx,eax
        and edx,0fffh           ;edx = low 12 bits of address
        cmp edx,0ff0h
        ja NearEndOfPage        ;jump if within 16 bytes of page boundary

; Test current text block for '\0' byte
        pcmpistri xmm1,[eax],14h            ;compare char range and text
        jnz @B                              ;jump if '\0' byte not found

; Found '\0' byte in current block (index in ECX)
; Calculate string length and return
        add eax,ecx                         ;eax = ptr to '\0' byte
        sub eax,[ebp+8]                     ;eax = final string length
        pop ebp
        ret

; Search for the '\0' terminator by examining each character
NearEndOfPage:
        mov ecx,4096                    ;ecx = size of page in bytes
        sub ecx,edx                     ;ecx = number of bytes to check

@@::    mov dl,[eax]                    ;dl = next text string character
        or dl,dl
        jz FoundNull                    ;jump if '\0' found
        inc eax                         ;eax = ptr to next char
        dec ecx
        jnz @B                          ;jump if more chars to test

; Remainder of text string can be searched using 16 byte blocks
; EAX is now aligned on a 16-byte boundary
        sub eax,16                          ;adjust eax for use in loop
@@:     add eax,16                          ;eax = ptr to next text block
        pcmpistri xmm1,[eax],14h            ;compare char range and text
        jnz @B                              ;jump if '\0' byte not found

; Found '\0' byte in current block (index in ECX)
        add eax,ecx                         ;eax = ptr to '\0' byte

; Calculate final string length and return
FoundNull:
        sub eax,[ebp+8]                     ;eax = final string length
        pop ebp
        ret
SseTextStringCalcLength_ endp
        end
