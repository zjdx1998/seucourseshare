        .model flat,c
        .const
DegToRad real8  0.01745329252
RadToDeg real8 57.2957795131
        .code

; extern "C" void RectToPolar_(double x, double y, double* r, double* a);
;
; Description:  This function converts a rectangular coordinate to a
;               to polar coordinate.

RectToPolar_ proc
        push ebp
        mov ebp,esp

; Calculate the angle.  Note that fpatan computes atan2(ST(1) / ST(0))
        fld real8 ptr [ebp+16]              ;load y
        fld real8 ptr [ebp+8]               ;load x
        fpatan                              ;calc atan2 (y / x)
        fmul [RadToDeg]                     ;convert angle to degrees
        mov eax,[ebp+28]
        fstp real8 ptr [eax]                ;save angle

; Calculate the radius
        fld real8 ptr [ebp+8]               ;load x
        fmul st(0),st(0)                    ;x * x
        fld real8 ptr [ebp+16]              ;load y
        fmul st(0),st(0)                    ;y * y
        faddp                               ;x * x + y * y
        fsqrt                               ;sqrt(x * x + y * y)
        mov eax,[ebp+24]
        fstp real8 ptr [eax]                ;save radius

        pop ebp
        ret
RectToPolar_    endp

; extern "C" void PolarToRect_(double r, double a, double* x, double* y);
;
; Description:  The following function converts a polar coordinate
;               to a rectangular coordinate.

PolarToRect_ proc
        push ebp
        mov ebp,esp

; Calculate sin(a) and cos(a).
; Following execution of fsincos, ST(0) = cos(a) and ST(1) = sin(a)
        fld real8 ptr [ebp+16]          ;load angle in degrees
        fmul [DegToRad]                 ;convert angle to radians
        fsincos                         ;calc sin(ST(0)) and cos(ST(0))

        fmul real8 ptr [ebp+8]              ;x = r * cos(a)
        mov eax,[ebp+24]
        fstp real8 ptr [eax]                ;save x

        fmul real8 ptr [ebp+8]              ;y = r * sin(a)
        mov eax,[ebp+28]
        fstp real8 ptr [eax]                ;save y

        pop ebp
        ret
PolarToRect_    endp
        end
