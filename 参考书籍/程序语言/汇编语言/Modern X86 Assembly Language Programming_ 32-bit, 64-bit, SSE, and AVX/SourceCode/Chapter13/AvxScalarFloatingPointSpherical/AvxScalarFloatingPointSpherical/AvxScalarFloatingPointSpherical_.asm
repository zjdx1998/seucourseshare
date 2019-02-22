                .model flat,c
                .const
Epsilon         real8 1.0e-15
r8_0p0          real8 0.0
r8_90p0         real8 90.0

                .code
                extern DegToRad:real8, RadToDeg:real8
                extern sin:proc, cos:proc, acos:proc, atan2:proc

; extern "C" bool RectToSpherical_(const double r_coord[3], double s_coord[3]);
;
; Description:  The following function performs rectangular to
;               spherical coordinate conversion.
;
; Requires: AVX

RectToSpherical_ proc
        push ebp
        mov ebp,esp
        push esi
        push edi
        sub esp,16                          ;space for acos & atan2 args

; Load argument values
        mov esi,[ebp+8]                     ;esi = ptr to r_coord
        mov edi,[ebp+12]                    ;edi = ptr to s_coord
        vmovsd xmm0,real8 ptr [esi]         ;xmm0 = x coord
        vmovsd xmm1,real8 ptr [esi+8]       ;xmm1 = y coord
        vmovsd xmm2,real8 ptr [esi+16]      ;xmm2 = z coord

; Compute r = sqrt(x * x + y * y + z * z)
        vmulsd xmm3,xmm0,xmm0               ;xmm3 = x * x
        vmulsd xmm4,xmm1,xmm1               ;xmm4 = y * y
        vmulsd xmm5,xmm2,xmm2               ;xmm5 = z * z
        vaddsd xmm6,xmm3,xmm4
        vaddsd xmm6,xmm6,xmm5
        vsqrtsd xmm7,xmm7,xmm6              ;xmm7 = r

; Compute phi = acos(z / r)
        vcomisd xmm7,real8 ptr [Epsilon]
        jae LB1                             ;jump if r >= epsilon
        vmovsd xmm4,real8 ptr [r8_0p0]      ;round r to 0.0
        vmovsd real8 ptr [edi],xmm4         ;save r
        vmovsd xmm4,real8 ptr [r8_90p0]     ;phi = 90.0 degrees
        vmovsd real8 ptr [edi+16],xmm4      ;save phi
        jmp LB2

LB1:    vmovsd real8 ptr [edi],xmm7         ;save r
        vdivsd xmm4,xmm2,xmm7               ;xmm4 = z / r
        vmovsd real8 ptr [esp],xmm4         ;save on stack
        call acos
        fmul real8 ptr [RadToDeg]           ;convert phi to degrees
        fstp real8 ptr [edi+16]             ;save phi

; Compute theta = atan2(y, x)
LB2:    vmovsd xmm0,real8 ptr [esi]         ;xmm0 = x
        vmovsd xmm1,real8 ptr [esi+8]       ;xmm1 = y
        vmovsd real8 ptr [esp+8],xmm0
        vmovsd real8 ptr [esp],xmm1
        call atan2
        fmul real8 ptr [RadToDeg]           ;convert theta to degrees
        fstp real8 ptr [edi+8]              ;save theta

        add esp,16
        pop edi
        pop esi
        pop ebp
        ret
RectToSpherical_ endp

; extern "C" bool SphericalToRect(const double s_coord[3], double r_coord[3]);
;
; Description:  The following function performs spherical to
;               rectangular coordinate conversion.
;
; Requires: AVX
;
; Local stack variables
;   ebp-8     sin(theta)
;   ebp-16    cos(theta)
;   ebp-24    sin(phi)
;   ebp-32    cos(phi)

SphericalToRect_ proc
        push ebp
        mov ebp,esp
        sub esp,32                          ;local variable space
        push esi
        push edi
        sub esp,8                           ;space for sin & cos argument

; Load argument values
        mov esi,[ebp+8]                     ;esi = ptr to s_coord
        mov edi,[ebp+12]                    ;edi = ptr to r_coord

; Compute sin(theta) and cos(theta)
        vmovsd xmm0,real8 ptr [esi+8]           ;xmm0 = theta
        vmulsd xmm1,xmm0,real8 ptr [DegToRad]   ;xmm1 = theta in radians
        vmovsd real8 ptr [ebp-16],xmm1          ;save theta for later use
        vmovsd real8 ptr [esp],xmm1
        call sin
        fstp real8 ptr [ebp-8]              ;save sin(theta)
        vmovsd xmm1,real8 ptr [ebp-16]      ;xmm1 = theta in radians
        vmovsd real8 ptr [esp],xmm1
        call cos
        fstp real8 ptr [ebp-16]             ;save cos(theta)

; Compute sin(phi) and cos(phi)
        vmovsd xmm0,real8 ptr [esi+16]          ;xmm0 = phi
        vmulsd xmm1,xmm0,real8 ptr [DegToRad]   ;xmm1 = phi in radians
        vmovsd real8 ptr [ebp-32],xmm1          ;save phi for later use
        vmovsd real8 ptr [esp],xmm1
        call sin
        fstp real8 ptr [ebp-24]             ;save sin(phi)
        vmovsd xmm1,real8 ptr [ebp-32]      ;xmm1 = phi in radians
        vmovsd real8 ptr [esp],xmm1
        call cos
        fstp real8 ptr [ebp-32]             ;save cos(phi)

; Compute x = r * sin(phi) * cos(theta)
        vmovsd xmm0,real8 ptr [esi]         ;xmm0 = r
        vmulsd xmm1,xmm0,real8 ptr [ebp-24] ;xmm1 = r * sin(phi)
        vmulsd xmm2,xmm1,real8 ptr [ebp-16] ;xmm2 = r*sin(phi)*cos(theta)
        vmovsd real8 ptr [edi],xmm2         ;save x

; Compute y = r * sin(phi) * sin(theta)
        vmulsd xmm2,xmm1,real8 ptr [ebp-8]  ;xmm2 = r*sin(phi)*sin(theta)
        vmovsd real8 ptr [edi+8],xmm2       ;save y

; Compute z = r * cos(phi)
        vmulsd xmm1,xmm0,real8 ptr [ebp-32] ;xmm1 = r * cos(phi)
        vmovsd real8 ptr [edi+16],xmm1      ;save z

        add esp,8
        pop edi
        pop esi
        mov esp,ebp
        pop ebp
        ret
SphericalToRect_ endp
        end
