        .model flat,c

; This structure must agree with the structure that's defined
; in file SseScalarFloatingPointParallelograms.cpp.
PDATA   struct
A       real8 ?
B       real8 ?
Alpha   real8 ?
Beta    real8 ?
H       real8 ?
Area    real8 ?
P       real8 ?
Q       real8 ?
BadVal  byte ?
Pad     byte 7 dup(?)
PDATA   ends

; Constant values used by function
            .const
            public SizeofPdataX86_
r8_2p0      real8 2.0
r8_180p0    real8 180.0
r8_MinusOne real8 -1.0
SizeofPdataX86_ dword size PDATA

            .code
            extern sin:proc, cos:proc
            extern DegToRad:real8

; extern "C" bool SseSfpParallelograms_(PDATA* pdata, int n);
;
; Description:  The following function calculates area and length
;               values for parallelograms.
;
; Returns:      0   n <= 0
;               1   n > 0
;
; Local stack:  [ebp-8]     x87 FPU transfer location
;               [ebp-16]    Alpha in radians
;
; Requires SSE2

SseSfpParallelograms_ proc
        push ebp
        mov ebp,esp
        sub esp,16                          ;allocate space for local vars
        push ebx

; Load arguments and validate n
        xor eax,eax                         ;set error code
        mov ebx,[ebp+8]                     ;ebx = pdata
        mov ecx,[ebp+12]                    ;ecx = n
        test ecx,ecx
        jle Done                            ;jump if n <= 0

; Initialize constant values
Loop1:  movsd xmm6,real8 ptr [r8_180p0]     ;xmm6 = 180.0
        xorpd xmm7,xmm7                     ;xmm7 = 0.0
        sub esp,8                           ;space for sin/cos arg value

; Load and validate A and B
        movsd xmm0,real8 ptr [ebx+PDATA.A]  ;xmm0 = A
        movsd xmm1,real8 ptr [ebx+PDATA.B]  ;xmm0 = B
        comisd xmm0,xmm7
        jp InvalidValue
        jbe InvalidValue                    ;jump if A <= 0.0
        comisd xmm1,xmm7
        jp InvalidValue
        jbe InvalidValue                    ;jump if B <= 0.0

; Load and validate Alpha
        movsd xmm2,real8 ptr [ebx+PDATA.Alpha]
        comisd xmm2,xmm7
        jp InvalidValue
        jbe InvalidValue                    ;jump if Alpha <= 0.0
        comisd xmm2,xmm6
        jae InvalidValue                    ;jump if Alpha >= 180.0

; Compute Beta
        subsd xmm6,xmm2                         ;Beta = 180.0 - Alpha
        movsd real8 ptr [ebx+PDATA.Beta],xmm6   ;Save Beta

; Compute sin(Alpha)
        mulsd xmm2,real8 ptr [DegToRad]     ;convert Alpha to radians
        movsd real8 ptr [ebp-16],xmm2       ;save value for later
        movsd real8 ptr [esp],xmm2          ;copy Alpha onto stack
        call sin
        fstp real8 ptr [ebp-8]              ;save sin(Alpha)

; Compute parallelogram Height and Area
        movsd xmm0,real8 ptr [ebx+PDATA.A]          ;A
        mulsd xmm0,real8 ptr [ebp-8]                ;A * sin(Alpha)
        movsd real8 ptr [ebx+PDATA.H],xmm0          ;save height
        mulsd xmm0,real8 ptr [ebx+PDATA.B]          ;A * sin(Alpha) * B
        movsd real8 ptr [ebx+PDATA.AREA],xmm0       ;save area

; Compute cos(Alpha)
        movsd xmm0,real8 ptr [ebp-16]           ;xmm0 = Alpha in radians
        movsd real8 ptr [esp],xmm0              ;copy Alpha onto stack
        call cos
        fstp real8 ptr [ebp-8]                  ;save cos(Alpha)

; Compute 2.0 * A * B * cos(Alpha)
        movsd xmm0,real8 ptr [r8_2p0]
        movsd xmm1,real8 ptr [ebx+PDATA.A]
        movsd xmm2,real8 ptr [ebx+PDATA.B]
        mulsd xmm0,xmm1                     ;2 * A
        mulsd xmm0,xmm2                     ;2 * A * B
        mulsd xmm0,real8 ptr [ebp-8]        ;2 * A * B * cos(Alpha)

; Compute A * A + B * B
        movsd xmm3,xmm1
        movsd xmm4,xmm2
        mulsd xmm3,xmm3                     ;A * A
        mulsd xmm4,xmm4                     ;B * B
        addsd xmm3,xmm4                     ;A * A + B * B
        movsd xmm4,xmm3                     ;A * A + B * B

; Compute P and Q
        subsd xmm3,xmm0
        sqrtsd xmm3,xmm3                        ;xmm3 = P
        movsd real8 ptr [ebx+PDATA.P],xmm3
        addsd xmm4,xmm0
        sqrtsd xmm4,xmm4                        ;xmm4 = Q
        movsd real8 ptr [ebx+PDATA.Q],xmm4
        mov dword ptr [ebx+PDATA.BadVal],0      ;set BadVal to false

NextItem:
        add ebx,size PDATA                  ;ebx = next element in array
        dec ecx
        jnz Loop1                           ;repeat loop until done

        add esp,8                           ;restore ESP
Done:   pop ebx
        mov esp,ebp
        pop ebp
        ret

; Set structure members to know values for display purposes
InvalidValue:
        movsd xmm0,real8 ptr [r8_MinusOne]
        movsd real8 ptr [ebx+PDATA.Beta],xmm0
        movsd real8 ptr [ebx+PDATA.H],xmm0
        movsd real8 ptr [ebx+PDATA.Area],xmm0
        movsd real8 ptr [ebx+PDATA.P],xmm0
        movsd real8 ptr [ebx+PDATA.Q],xmm0
        mov dword ptr [ebx+PDATA.BadVal],1
        jmp NextItem

SseSfpParallelograms_ endp
        end
