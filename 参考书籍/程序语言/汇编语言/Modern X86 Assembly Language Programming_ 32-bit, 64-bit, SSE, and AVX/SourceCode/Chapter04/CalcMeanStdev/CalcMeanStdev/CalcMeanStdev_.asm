        .model flat,c
        .code

; extern "C" bool CalcMeanStdev(const double* a, int n, double* mean, double* stdev);
;
; Description:  The following function calculates the mean and
;               standard deviation of the values in an array.
;
; Returns:      0 = invalid 'n'
;               1 = valid 'n' 

CalcMeanStdev_  proc
        push ebp
        mov ebp,esp
        sub esp,4

; Make sure 'n' is valid
        xor eax,eax
        mov ecx,[ebp+12]
        cmp ecx,1
        jle Done                            ;jump if n <= 1
        dec ecx
        mov [ebp-4],ecx                     ;save n - 1 for later
        inc ecx

; Compute sample mean
        mov edx,[ebp+8]                     ;edx = 'a'
        fldz                                ;sum = 0.0

@@:     fadd real8 ptr [edx]                ;sum += *a
        add edx,8                           ;a++
        dec ecx
        jnz @B
        fidiv dword ptr [ebp+12]            ;mean = sum / n

; Compute sample stdev
        mov edx,[ebp+8]                     ;edx = 'a'
        mov ecx,[ebp+12]                    ;n
        fldz                                ;sum = 0.0, ST(1) = mean

@@:     fld real8 ptr [edx]                 ;ST(0) = *a,
        fsub st(0),st(2)                    ;ST(0) = *a - mean
        fmul st(0),st(0)                    ;ST(0) = (*a - mean) ^ 2
        faddp                               ;update sum
        add edx,8
        dec ecx
        jnz @B
        fidiv dword ptr [ebp-4]             ;var = sum / (n - 1)
        fsqrt                               ;final stdev

; Save results
        mov eax,[ebp+20]
        fstp real8 ptr [eax]                ;save stddev
        mov eax,[ebp+16]
        fstp real8 ptr [eax]                ;save mean
        mov eax,1                           ;set success return code

Done:   mov esp,ebp
        pop ebp
        ret
CalcMeanStdev_ endp
        end
