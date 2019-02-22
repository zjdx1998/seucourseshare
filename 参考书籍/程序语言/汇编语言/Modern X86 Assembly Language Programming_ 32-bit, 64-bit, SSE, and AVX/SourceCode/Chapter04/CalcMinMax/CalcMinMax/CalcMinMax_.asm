        .model flat,c
        .const
r4_MinFloat dword 0ff7fffffh                ;smallest float number
r4_MaxFloat dword  7f7fffffh                ;largest float number
        .code

; extern "C" bool CalcMinMax_(const float* a, int n, float* min, float* max);
;
; Description:  The following function calculates the min and max values
;               of a single-precision floating-point array.
;
; Returns:      0 = invalid 'n'
;               1 = valid 'n'

CalcMinMax_ proc
        push ebp
        mov ebp,esp

; Load argument values and make sure 'n' is valid.
        xor eax,eax                         ;set error return code
        mov edx,[ebp+8]                     ;edx = 'a'
        mov ecx,[ebp+12]                    ;ecx = 'n'
        test ecx,ecx
        jle Done                            ;jump if 'n' <= 0

        fld [r4_MinFloat]                  ;initial max_a value
        fld [r4_MaxFloat]                  ;initial min_a value

; Find min and max of input array
@@:     fld real4 ptr [edx]                 ;load *a
        fld st(0)                           ;duplicate *a on stack

        fcomi st(0),st(2)                   ;compare *a with min
        fcmovnb st(0),st(2)                 ;ST(0) equals smaller val
        fstp st(2)                          ;save new min value

        fcomi st(0),st(2)                   ;compare *a with max_a
        fcmovb st(0),st(2)                  ;st(0) equals larger val
        fstp st(2)                          ;save new max value

        add edx,4                           ;point to next a[i]
        dec ecx
        jnz @B                              ;repeat loop until finished

; Save results
        mov eax,[ebp+16]
        fstp real4 ptr [eax]                ;save final min
        mov eax,[ebp+20]    
        fstp real4 ptr [eax]                ;save final max
        mov eax,1                           ;set success return code

Done:   pop ebp
        ret
CalcMinMax_ endp
        end
