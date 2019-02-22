        .model flat,c
        .code

; extern "C" void AvxGprCountZeroBits_(Uint32 x, Uint32* lzcnt, Uint32* tzcnt);
;
; Description:  The following function demonstrates use of the lzcnt and
;               tzcnt instructions.
;
; Requires:     BMI1, LZCNT

AvxGprCountZeroBits_ proc
        mov eax,[esp+4]                     ;eax = x

        lzcnt ecx,eax                       ;count leading zeros
        mov edx,[esp+8]
        mov [edx],ecx                       ;save result

        tzcnt ecx,eax                       ;count trailing zeros
        mov edx,[esp+12]
        mov [edx],ecx                       ;save result
        ret
AvxGprCountZeroBits_ endp

; extern "C" Uint32 AvxGprBextr_(Uint32 x, Uint8 start, Uint8 length);
;
; Description:  The following function demonstrates use of the
;               bextr instruction.
;
; Requires:     BMI1

AvxGprBextr_ proc
        mov cl,[esp+8]                      ;cl = start index
        mov ch,[esp+12]                     ;ch = length of bit field
        bextr eax,[esp+4],ecx               ;eax = extracted bit field
        ret
AvxGprBextr_ endp

; extern "C" Uint32 AvxGprAndNot_(Uint32 x, Uint32 y);
;
; Description:  The following function demonstrates use of the
;               andn instruction.
;
; Requires:     BMI1

AvxGprAndNot_ proc
        mov ecx,[esp+4]
        andn eax,ecx,[esp+8]                ;eax = ~ecx & [esp+8]
        ret
AvxGprAndNot_ endp
        end
