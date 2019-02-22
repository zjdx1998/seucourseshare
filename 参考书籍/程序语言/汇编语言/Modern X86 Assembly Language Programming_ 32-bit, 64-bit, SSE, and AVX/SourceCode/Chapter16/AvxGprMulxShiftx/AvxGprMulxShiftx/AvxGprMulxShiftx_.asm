        .model flat,c
        .code

; extern "C" Uint64 AvxGprMulx_(Uint32 a, Uint32 b, Uint8 flags[2]);
;
; Description:  The following function demonstrates use of the
;               flagless unsigned integer multiply instruction mulx.
;
; Requires      BMI2.

AvxGprMulx_ proc
        push ebp
        mov ebp,esp

; Save copy of status flags before mulx
        mov ecx,[ebp+16]
        lahf
        mov byte ptr [ecx],ah

; Perform flagless multiplication.  The mulx instruction below computes
; the product of explicit source operand [ebp+8] and implicit source
; operand edx. The 64-bit result is saved to the register pair edx:eax.
        mov edx,[ebp+12]                    ;edx = b
        mulx edx,eax,[ebp+8]                ;edx:eax = [ebp+8] * edx

; Save copy of status flags after mulx
        push eax
        lahf
        mov byte ptr [ecx+1],ah
        pop eax

        pop ebp
        ret
AvxGprMulx_ endp

; extern "C" void AvxGprShiftx_(Int32 x, Uint32 count, Int32 results[3]);
;
; Description:  The following function demonstrates use of the flagless
;               shift instructions sarx, shlx, and shrx.
;
; Requires      BMI2

AvxGprShiftx_ proc
        push ebp
        mov ebp,esp

; Load argument values and perform shifts.  Note that each shift
; instruction requires three operands: DesOp, SrcOp, and CountOp.
        mov ecx,[ebp+12]            ;ecx = shift bit count
        mov edx,[ebp+16]            ;edx = ptr to results

        sarx eax,[ebp+8],ecx        ;shift arithmetic right
        mov [edx],eax
        shlx eax,[ebp+8],ecx        ;shift logical left
        mov [edx+4],eax
        shrx eax,[ebp+8],ecx        ;shift logical right
        mov [edx+8],eax

        pop ebp
        ret
AvxGprShiftx_ endp
        end
