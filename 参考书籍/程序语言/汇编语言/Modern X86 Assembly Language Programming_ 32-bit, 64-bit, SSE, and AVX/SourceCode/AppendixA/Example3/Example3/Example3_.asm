        .code

; extern "C" double CalcResult3_(int a, int b, double c, double d);

CalcResult3_ proc
        imul rcx,rcx                        ;ecx = a * a
        imul rdx,rdx                        ;edx = b * b

        cvtsi2sd xmm0,rcx                   ;convert rcx to DPFP
        cvtsi2sd xmm1,rdx                   ;convert rdx to DPFP

        mulsd xmm2,xmm2                     ;xmm2 = c * c
        mulsd xmm3,xmm3                     ;xmm3 = d * d

        addsd xmm0,xmm1                     ;xmm0 = a * a + b * b
        addsd xmm2,xmm3                     ;xmm2 = c * c + d * d
        addsd xmm0,xmm2                     ;xmm0 = sum of squares
        sqrtsd xmm0,xmm0                    ;xmm0 = sqrt(sum of squares)
        ret
CalcResult3_ endp
        end
