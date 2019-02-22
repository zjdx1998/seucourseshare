        .model flat,c
        .code

; extern "C" void MmxMulSignedWord_(MmxVal a, MmxVal b, MmxVal* prod_lo, MmxVal* prod_hi)
;
; Description:  The following function performs a SIMD multiplication of
;               two packed signed word operands.  The resultant doubleword
;               products are saved to the specified memory locations.

MmxMulSignedWord_ proc
        push ebp
        mov ebp,esp

; Load arguments 'a' and 'b'
        movq mm0,[ebp+8]                    ;mm0 = 'a'
        movq mm1,[ebp+16]                   ;mm1 = 'b'

; Perform packed signed integer word multiplication
        movq mm2,mm0                        ;mm2 = 'a'
        pmullw mm0,mm1                      ;mm0 = product low result
        pmulhw mm1,mm2                      ;mm1 = product high result

; Unpack and interleave low and high products to form
; final packed doubleword products
        movq mm2,mm0                        ;mm2 = product low result
        punpcklwd mm0,mm1                   ;mm0 = low dword products
        punpckhwd mm2,mm1                   ;mm2 = high dword products

; Save the packed doubleword results
        mov eax,[ebp+24]                    ;eax = pointer to 'prod_lo'
        mov edx,[ebp+28]                    ;edx = pointer to 'prod_hi'
        movq [eax],mm0                      ;save low dword products
        movq [edx],mm2                      ;save high dword products

        pop ebp
        ret
MmxMulSignedWord_ endp
        end
