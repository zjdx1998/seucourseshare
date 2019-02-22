        include <MacrosX86-64.inc>
        .code
        extern Mat4x4Trace_:proc
        extern Mat4x4Mul_:proc

; extern "C" void TestMat4x4(Mat4x4 m1, Mat4x4 m2, float* m1_trace, Mat4x4 m3);

TestMat4x4 proc frame
        _CreateFrame TestMat4x4_,16,160
        _SaveXmmRegs xmm6,xmm7,xmm8,xmm9,xmm10,xmm11,xmm12,xmm13,xmm14,xmm15
        _EndProlog

        vmovaps ymm0,ymmword ptr [rcx]
        vmovaps ymm1,ymmword ptr [rcx+32]
        call Mat4x4Trace_
        movss real4 ptr [r8],xmm0

        vmovaps ymm0,ymmword ptr [rcx]
        vmovaps ymm1,ymmword ptr [rcx+32]
        vmovaps ymm2,ymmword ptr [rdx]
        vmovaps ymm3,ymmword ptr [rdx+32]
        call Mat4x4Mul_
        vmovaps ymmword ptr [r9],ymm0
        vmovaps ymmword ptr [r9+32],ymm1

Done:    vzeroupper
        _RestoreXmmRegs xmm6,xmm7,xmm8,xmm9,xmm10,xmm11,xmm12,xmm13,xmm14,xmm15
        _DeleteFrame
        ret
TestMat4x4 endp
        end