        .model flat,c
        .code

; extern "C" MmxVal MmxAdd_(MmxVal a, MmxVal b, MmxAddOp add_op);
;
; Description:  The following function demonstrates use of the
;               padd* instructions.
;
; Returns:      Register pair edx:eax contains the calculated result.

MmxAdd_     proc
            push ebp
            mov ebp,esp

; Make sure 'add_op' is valid
            mov eax,[ebp+24]                ;load 'add_op'
            cmp eax,AddOpTableCount         ;compare to table count
            jae BadAddOp                    ;jump if 'add_op' is invalid

; Load parameters and execute specified instruction
            movq mm0,[ebp+8]                ;load 'a'
            movq mm1,[ebp+16]               ;load 'b'
            jmp [AddOpTable+eax*4]          ;jump to specified 'add_op'

MmxPaddb:   paddb mm0,mm1                   ;packed byte addition using
            jmp SaveResult                  ;wraparound

MmxPaddsb:  paddsb mm0,mm1                  ;packed byte addition using
            jmp SaveResult                  ;signed saturation

MmxPaddusb: paddusb mm0,mm1                 ;packed byte addition using
            jmp SaveResult                  ;unsigned saturation

MmxPaddw:   paddw mm0,mm1                   ;packed word addition using
            jmp SaveResult                  ;wraparound

MmxPaddsw:  paddsw mm0,mm1                  ;packed word addition using
            jmp SaveResult                  ;signed saturation

MmxPaddusw: paddusw mm0,mm1                 ;packed word addition using
            jmp SaveResult                  ;unsigned saturation

MmxPaddd:   paddd mm0,mm1                   ;packed dword addition using
            jmp SaveResult                  ;wraparound

BadAddOp:   pxor mm0,mm0                    ;return 0 if 'add_op' is bad

; Move final result into edx:eax
SaveResult: movd eax,mm0                    ;eax = low dword of mm0
            pshufw mm2,mm0,01001110b        ;swap high & low dwords
            movd edx,mm2                    ;edx:eax = final result

            emms                            ;clear MMX state
            pop ebp
            ret

; The order of the labels in the following table must match
; the enum that is defined in MmxAddition.cpp.

        align 4
AddOpTable:
        dword MmxPaddb, MmxPaddsb, MmxPaddusb
        dword MmxPaddw, MmxPaddsw, MmxPaddusw, MmxPaddd
AddOpTableCount equ ($ - AddOpTable) / size dword

MmxAdd_ endp
        end
