        .model flat,c
        .code

; extern "C" bool MmxShift_(MmxVal a, MmxShiftOp shift_op, int count, MmxVal* b);
;
; Description:  The following function demonstrates use of various MMX
;               shift instructions.
;
; Returns:      0 = invalid 'shift_op' argument
;               1 = success

MmxShift_   proc
            push ebp
            mov ebp,esp

; Make sure 'shift_op' is valid
            xor eax,eax                     ;set error code
            mov edx,[ebp+16]                ;load 'shift_op'
            cmp edx,ShiftOpTableCount       ;compare against table count
            jae BadShiftOp                  ;jump if 'shift_op' invalid

; Jump to the specfied shift operation
            mov eax,1                       ;set success return code
            movq mm0,[ebp+8]                ;load 'a'
            movd mm1,dword ptr [ebp+20]     ;load 'count' into low dword
            jmp [ShiftOpTable+edx*4]

MmxPsllw:   psllw mm0,mm1                   ;shift left logical word
            jmp SaveResult

MmxPsrlw:   psrlw mm0,mm1                   ;shift right logical word
            jmp SaveResult

MmxPsraw:   psraw mm0,mm1                   ;shift right arithmetic word
            jmp SaveResult

MmxPslld:   pslld mm0,mm1                   ;shift left logical dword
            jmp SaveResult

MmxPsrld:   psrld mm0,mm1                   ;shift right logical dword
            jmp SaveResult

MmxPsrad:   psrad mm0,mm1                   ;shift right arithmetic dword
            jmp SaveResult

BadShiftOp: pxor mm0,mm0                    ;use 0 if 'shift_op' is bad

SaveResult: mov edx,[ebp+24]                ;edx = ptr to 'b'
            movq [edx],mm0                  ;save shift result
            emms                            ;clear MMX state

            pop ebp
            ret

; The order of the labels in the following table must correspond
; to the enum that is defined in MmxShift.cpp.

            align 4
ShiftOpTable:
            dword MmxPsllw, MmxPsrlw, MmxPsraw
            dword MmxPslld, MmxPsrld, MmxPsrad
ShiftOpTableCount equ ($ - ShiftOpTable) / size dword

MmxShift_ endp
        end
