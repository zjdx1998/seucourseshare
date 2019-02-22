        .model flat,c
        .code

; extern "C" int SignedMinA_(int a, int b, int c);
;
; Description:  Determines minimum of three signed integers
;               using conditional jumps.
;
; Returns       min(a, b, c)

SignedMinA_ proc
        push ebp
        mov ebp,esp
        mov eax,[ebp+8]                     ;eax = 'a'
        mov ecx,[ebp+12]                    ;ecx = 'b'

; Determine min(a, b)
        cmp eax,ecx
        jle @F
        mov eax,ecx                         ;eax = min(a, b)

; Determine min(a, b, c)
@@:     mov ecx,[ebp+16]                    ;ecx = 'c'
        cmp eax,ecx
        jle @F
        mov eax,ecx                         ;eax = min(a, b, c)

@@:     pop ebp
        ret
SignedMinA_ endp

; extern "C" int SignedMaxA_(int a, int b, int c);
;
; Description:  Determines maximum of three signed integers
;               using conditional jumps.
;
; Returns:      max(a, b, c)

SignedMaxA_ proc
        push ebp
        mov ebp,esp
        mov eax,[ebp+8]                     ;eax = 'a'
        mov ecx,[ebp+12]                    ;ecx = 'b'

        cmp eax,ecx
        jge @F
        mov eax,ecx                         ;eax = max(a, b)

@@:     mov ecx,[ebp+16]                    ;ecx = 'c'
        cmp eax,ecx
        jge @F
        mov eax,ecx                         ;eax = max(a, b, c)

@@:     pop ebp
        ret
SignedMaxA_ endp

; extern "C" int SignedMinB_(int a, int b, int c);
;
; Description:  Determines minimum of three signed integers
;               using conditional moves.
;
; Returns       min(a, b, c)

SignedMinB_ proc
        push ebp
        mov ebp,esp
        mov eax,[ebp+8]                     ;eax = 'a'
        mov ecx,[ebp+12]                    ;ecx = 'b'

; Determine smallest value using the CMOVG instruction
        cmp eax,ecx
        cmovg eax,ecx                       ;eax = min(a, b)
        mov ecx,[ebp+16]                    ;ecx = 'c'
        cmp eax,ecx
        cmovg eax,ecx                       ;eax = min(a, b, c)

        pop ebp
        ret
SignedMinB_ endp

; extern "C" int SignedMaxB_(int a, int b, int c);
;
; Description:  Determines maximum of three signed integers
;               using conditional moves.
;
; Returns:      max(a, b, c)

SignedMaxB_ proc
        push ebp
        mov ebp,esp
        mov eax,[ebp+8]                     ;eax = 'a'
        mov ecx,[ebp+12]                    ;ecx = 'b'

; Determine largest value using the CMOVL instruction
        cmp eax,ecx
        cmovl eax,ecx                       ;eax = max(a, b)
        mov ecx,[ebp+16]                    ;ecx = 'c'
        cmp eax,ecx
        cmovl eax,ecx                       ;eax = max(a, b, c)

        pop ebp
        ret
SignedMaxB_ endp
        end
