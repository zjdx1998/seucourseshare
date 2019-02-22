        .model flat,c
        .const
r8_4p0  real8 4.0
r8_3p0  real8 3.0

; extern "C" bool CalcSphereAreaVolume_(double r, double* sa, double* v);
;
; Description:  This function calculates the surface area and volume
;               of a sphere.
;
; Returns:      0 = invalid radius
;               1 = valid radius

        .code
CalcSphereAreaVolume_ proc
        push ebp
        mov ebp,esp

; Make sure radius is valid
        xor eax,eax                         ;set error return code
        fld real8 ptr [ebp+8]               ;ST(0) = r
        fldz                                ;ST(0) = 0.0, ST(1) = r
        fcomip st(0),st(1)                  ;compare 0.0 to r
        fstp st(0)                          ;remove r from stack
        jp Done                             ;jump if unordered operands
        ja Done                             ;jump if r < 0.0

; Calculate sphere surface area
        fld real8 ptr [ebp+8]               ;ST(0) = r
        fld st(0)                           ;ST(0) = r, ST(1) = r
        fmul st(0),st(0)                    ;ST(0) = r * r, ST(1) = r
        fldpi                               ;ST(0) = pi
        fmul [r8_4p0]                       ;ST(0) = 4 * pi
        fmulp                               ;ST(0) = 4 * pi * r * r

        mov edx,[ebp+16]
        fst real8 ptr [edx]                 ;save surface area

; Calculate sphere volume
        fmulp                           ;ST(0) = pi * 4 * r * r * r
        fdiv [r8_3p0]                   ;ST(0) = pi * 4 * r * r * r / 3

        mov edx,[ebp+20]
        fstp real8 ptr [edx]                ;save volume
        mov eax,1                           ;set success return code

Done:   pop ebp
        ret
CalcSphereAreaVolume_ endp
        end
