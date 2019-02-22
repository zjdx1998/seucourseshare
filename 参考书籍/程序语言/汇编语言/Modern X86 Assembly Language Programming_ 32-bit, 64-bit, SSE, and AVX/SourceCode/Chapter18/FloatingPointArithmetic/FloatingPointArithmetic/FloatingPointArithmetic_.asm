        .code

; extern "C" double CalcSum_(float a, double b, float c, double d, float e, double f);
;
; Description:  The following function demonstrates how to access
;               floating-point argument values in an x86-64 function.

CalcSum_ proc

; Sum the argument values
        cvtss2sd xmm0,xmm0                  ;promote a to DPFP
        addsd xmm0,xmm1                     ;xmm0 = a + b

        cvtss2sd xmm2,xmm2                  ;promote c to DPFP
        addsd xmm0,xmm2                     ;xmm0 = a + b + c
        addsd xmm0,xmm3                     ;xmm0 = a + b + c + d

        cvtss2sd xmm4,real4 ptr [rsp+40]    ;promote e to DPFP
        addsd xmm0,xmm4                     ;xmm0 = a + b + c + d + e

        addsd xmm0,real8 ptr [rsp+48]       ;xmm0 =  a + b + c + d + e + f

        ret
CalcSum_ endp

; extern "C" double CalcDist_(int x1, double x2, long long y1, double y2, float z1, short z2);
;
; Description:  The following function demonstrates how to access mixed
;               floating-point and integer arguments values in an
;               x86-64 function.

CalcDist_ proc

; Calculate xd = (x2 - x1) * (x2 - x1)
        cvtsi2sd xmm4,ecx                   ;convert x1 to DPFP
        subsd xmm1,xmm4                     ;xmm1 = x2 - x1
        mulsd xmm1,xmm1                     ;xmm1 = xd

; Calculate yd = (y2 - y1) * (y2 - y1)
        cvtsi2sd xmm5,r8                    ;convert y1 to DPFP
        subsd xmm3,xmm5                     ;xmm3 = y2 - y1
        mulsd xmm3,xmm3                     ;xmm3 = yd

; Calculate zd = (z2 - z1) * (z2 - z1)
        movss xmm0,real4 ptr [rsp+40]       ;xmm=0  = z1
        cvtss2sd xmm0,xmm0                  ;convert z1 to DPFP
        movsx eax,word ptr [rsp+48]         ;eax = sign-extend z2
        cvtsi2sd xmm4,eax                   ;convert z2 to DPFP
        subsd xmm4,xmm0                     ;xmm4 = z2 - z1
        mulsd xmm4,xmm4                     ;xmm4 = zd

; Calculate final distance sqrt(xd + yd + zd)
        addsd xmm1,xmm3                     ;xmm1 = xd + yd
        addsd xmm4,xmm1                     ;xmm4 = xd + yd + zd
        sqrtsd xmm0,xmm4                    ;xmm0 = sqrt(xd + yd + zd)

        ret
CalcDist_ endp
        end
