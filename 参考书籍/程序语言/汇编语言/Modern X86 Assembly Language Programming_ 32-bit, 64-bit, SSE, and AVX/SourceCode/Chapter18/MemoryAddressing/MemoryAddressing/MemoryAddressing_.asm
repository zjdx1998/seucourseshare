; Simple lookup table (.const section data is read only)

            .const
FibVals     dword 0, 1, 1, 2, 3, 5, 8, 13
            dword 21, 34, 55, 89, 144, 233, 377, 610, 987, 1597
NumFibVals_ dword ($ - FibVals) / sizeof dword
            public NumFibVals_

            .data
FibValsSum_ dword ?             ;value to demo RIP-relative addressing
            public FibValsSum_

           .code

; extern "C" int MemoryAddressing_(int i, int* v1, int* v2, int* v3, int* v4);
;
; Description:  This function demonstrates various addressing modes
;               that can be used to access operands in memory.
;
; Returns:      0 = error (invalid table index)
;               1 = success

MemoryAddressing_ proc

; Make sure 'i' is valid
        cmp ecx,0
        jl InvalidIndex                     ;jump if i < 0
        cmp ecx,[NumFibVals_]
        jge InvalidIndex                    ;jump if i >= NumFibVals_

; Sign extend i for use in address calculations
        movsxd rcx,ecx                      ;sign extend i
        mov [rsp+8],rcx                     ;save copy of i

; Example #1 - base register
        mov r11,offset FibVals              ;r11 = FibVals
        shl rcx,2                           ;rcx = i * 4
        add r11,rcx                         ;r11 = FibVals + i * 4
        mov eax,[r11]                       ;eax = FibVals[i]
        mov [rdx],eax                       ;Save to v1

; Example #2 - base register + index register
        mov r11,offset FibVals              ;r11 = FibVals
        mov rcx,[rsp+8]                     ;rcx = i
        shl rcx,2                           ;rcx = i * 4
        mov eax,[r11+rcx]                   ;eax = FibVals[i]
        mov [r8],eax                        ;Save to v2

; Example #3 - base register + index register * scale factor
        mov r11,offset FibVals              ;r11 = FibVals
        mov rcx,[rsp+8]                     ;rcx = i
        mov eax,[r11+rcx*4]                 ;eax = FibVals[i]
        mov [r9],eax                        ;Save to v3

; Example #4 - base register + index register * scale factor + disp
        mov r11,offset FibVals-42           ;r11 = FibVals - 42
        mov rcx,[rsp+8]                     ;rcx = i
        mov eax,[r11+rcx*4+42]              ;eax = FibVals[i]
        mov r10,[rsp+40]                    ;r10 = ptr to v4
        mov [r10],eax                       ;Save to v4

; Example #5 - RIP relative
        add [FibValsSum_],eax               ;Update sum

        mov eax,1                           ;set success return code
        ret

InvalidIndex:
        xor eax,eax                         ;set error return code
        ret

MemoryAddressing_ endp
        end
