        .code

; extern "C" Int64 IntegerAdd_(Int64 a, Int64 b, Int64 c, Int64 d, Int64 e, Int64 f)
;
; Description:  The following function demonstrates 64-bit integer
;               addition.

IntegerAdd_ proc

; Calculate sum of argument values
        add rcx,rdx                         ;rcx = a + b
        add r8,r9                           ;r8 = c + d

        mov rax,[rsp+40]                    ;rax = e
        add rax,[rsp+48]                    ;rax = e + f

        add rcx,r8                          ;rcx = a + b + c + d
        add rax,rcx                         ;rax = a + b + c + d + e + f

        ret
IntegerAdd_ endp

; extern "C" Int64 IntegerMul_(Int8 a, Int16 b, Int32 c, Int64 d, Int8 e, Int16 f, Int32 g, Int64 h);
;
; Description:  The following function demonstrates 64-bit signed
;               integer multiplication.

IntegerMul_ proc

; Calculate a * b
        movsx r10,cl                        ;r10 = sign_extend(a)
        movsx r11,dx                        ;r11 = sign_extend(b)
        imul r10,r11                        ;r10 = a * b

;Calculate c * d
        movsxd rcx,r8d                      ;rcx = sign_extend(c)
        imul rcx,r9                         ;rcx = c * d

; Calculate e * f
        movsx r8,byte ptr [rsp+40]          ;r8 = sign_extend(e)
        movsx r9,word ptr [rsp+48]          ;r9 = sign_extend(f)
        imul r8,r9                          ;r8 = e * f

; Calculate g * h
        movsxd rax,dword ptr [rsp+56]       ;rax = sign_extend (g)
        imul rax,[rsp+64]                   ;rax = g * h

; Compute final result
        imul r10,rcx                        ;r10 = a * b * c * d
        imul rax,r8                         ;rax = e * f * g * h
        imul rax,r10                        ;rax = final product

        ret
IntegerMul_ endp

; extern "C" void IntegerDiv_(Int64 a, Int64 b, Int64 quo_rem_ab[2], Int64 c, Int64 d, Int64 quo_rem_cd[2]);
;
; Description:  The following function demonstrates 64-bit signed
;               integer division.

IntegerDiv_ proc

; Calculate a / b, save quotient and remainder
        mov [rsp+16],rdx                    ;save b
        mov rax,rcx                         ;rax = a
        cqo                                 ;rdx:rax = sign_extend(a)
        idiv qword ptr [rsp+16]             ;rax = quo a/b, rdx = rem a/b
        mov [r8],rax                        ;save quotient
        mov [r8+8],rdx                      ;save remainder

; Calculate c / d, save quotient and remainder
        mov rax,r9                          ;rax = c
        cqo                                 ;rdx:rax = sign_extend(c)
        idiv qword ptr [rsp+40]             ;rax = quo c/d, rdx = rem c/d
        mov r10,[rsp+48]                    ;r10 = ptr to quo_rem_cd
        mov [r10],rax                       ;save quotient
        mov [r10+8],rdx                     ;save remainder

        ret
IntegerDiv_ endp
        end
