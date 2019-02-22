            .model flat,c

; Constants required to calculate sphere surface area and volume.
            .const
r8_pi       real8 3.14159265358979323846
r8_four     real8 4.0
r8_three    real8 3.0
r8_neg_one  real8 -1.0
            .code

; extern "C" bool SseSfpCalcSphereAreaVolume_(double r, double* sa, double* v);
;
; Description:  The following function calculates the surface area and
;               volume of a sphere.
;
; Returns:      0 = invalid radius
;               1 = success
;
; Requires:     SSE2

SseSfpCalcSphereAreaVolume_ proc
        push ebp
        mov ebp,esp

; Load arguments and make sure radius is valid
        movsd xmm0,real8 ptr [ebp+8]        ;xmm0 = r
        mov ecx,[ebp+16]                    ;ecx = sa
        mov edx,[ebp+20]                    ;edx = v
        xorpd xmm7,xmm7                     ;xmm7 = 0.0
        comisd xmm0,xmm7                    ;compare r against 0.0
        jp BadRadius                        ;jump if r is NAN
        jb BadRadius                        ;jump if r < 0.0

; Compute the surface area
        movsd xmm1,xmm0                     ;xmm1 = r
        mulsd xmm1,xmm1                     ;xmm1 = r * r
        mulsd xmm1,[r8_four]                ;xmm1 =  4 * r * r
        mulsd xmm1,[r8_pi]                  ;xmm1 =  4 * pi r * r
        movsd real8 ptr [ecx],xmm1          ;save surface area

; Compute the volume
        mulsd xmm1,xmm0                 ;xmm1 =  4 * pi * r * r * r
        divsd xmm1,[r8_three]           ;xmm1 =  4 * pi * r * r * r / 3
        movsd real8 ptr [edx],xmm1      ;save volume
        mov eax,1                       ;set success return code
        pop ebp
        ret

; Invalid radius - set surface area and volume to -1.0
BadRadius:
        movsd xmm0,[r8_neg_one]             ;xmm0 = -1.0
        movsd real8 ptr [ecx],xmm0          ;*sa = -1.0
        movsd real8 ptr [edx],xmm0          ;*v = -1.0;
        xor eax,eax                         ;set error return code
        pop ebp
        ret
SseSfpCalcSphereAreaVolume_ endp
        end
