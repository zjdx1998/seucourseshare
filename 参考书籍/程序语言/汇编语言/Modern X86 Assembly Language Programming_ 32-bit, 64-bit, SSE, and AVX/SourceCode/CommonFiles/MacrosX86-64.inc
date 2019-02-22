        .nolist
        extern __chkstk:proc

;-------------------------------------------------------------------------
; X86-64 Macro Notes
;
; The macros defined in this file assume that the stack frame is
; organized as described in the text.
;
; A VS project rebuild may be necessary if any changes are made
; in this file.
;
; A function must use the macros in the following order:
;   _CreateFrame        required
;   _SaveXmmRegs        optional
;   _EndProlog          required
;   _RestoreXmmRegs     optional
;   _DeleteFrame        required
;-------------------------------------------------------------------------

;-------------------------------------------------------------------------
; _CreateFrame
;
; Description:  The following macro generates code that creates a stack
;               frame for x86-64 functions.
;
; Macro Parameters: Prefix          Macro symbolic name prefix
;                   StkSizeLocal1   Size in bytes for local1 vars
;                   StkSizeLocal2   Size in bytes for XMM reg save area
;                   Rnv1 - Rnv7     Non-volatile registers to save
;-------------------------------------------------------------------------

_CreateFrame macro Prefix,StkSizeLocal1,StkSizeLocal2,Rnv1,Rnv2,Rnv3,Rnv4,Rnv5,Rnv6,Rnv7

; Make sure stack sizes are valid
.errnz StkSizeLocal1 MOD 16, <Macro parameter 'StkSizeLocal1' must be evenly divisible by 16>
.errnz StkSizeLocal2 MOD 16, <Macro parameter 'StkSizeLocal2' must be evenly divisible by 16>
.errnz StkSizeLocal2 GT 240, <Macro parameter 'StkSizeLocal2' must be less than or equal to 240>

        push rbp
       .pushreg rbp
        NumPushReg=1
IFNB <Rnv1>
        push Rnv1
        .pushreg Rnv1
        NumPushReg=NumPushReg+1
ENDIF
IFNB <Rnv2>
        push Rnv2
        .pushreg Rnv2
        NumPushReg=NumPushReg+1
ENDIF
IFNB <Rnv3>
        push Rnv3
        .pushreg Rnv3
        NumPushReg=NumPushReg+1
ENDIF
IFNB <Rnv4>
        push Rnv4
        .pushreg Rnv4
        NumPushReg=NumPushReg+1
ENDIF
IFNB <Rnv5>
        push Rnv5
        .pushreg Rnv5
        NumPushReg=NumPushReg+1
ENDIF
IFNB <Rnv6>
        push Rnv6
        .pushreg Rnv6
        NumPushReg=NumPushReg+1
ENDIF
IFNB <Rnv7>
        push Rnv7
        .pushreg Rnv7
        NumPushReg=NumPushReg+1
ENDIF

        StackPad=((NumPushReg AND 1) XOR 1) * 8
        StackSizeTotal=StkSizeLocal1+StkSizeLocal2+StackPad

; Call helper function __chkstk if StackSizeTotal >= 4096 bytes (1 page)
; See Visual Studio documentation for more information
IF (StackSizeTotal GE 4096)
        mov rax,StackSizeTotal
        call __chkstk
        sub rsp,rax
        .allocstack StackSizeTotal
ELSEIF (StackSizeTotal GT 0)
        sub rsp,StackSizeTotal
        .allocstack StackSizeTotal
ENDIF

IF (StkSizeLocal2 GT 0)
        lea rbp,[rsp+StkSizeLocal2]
ELSE
        mov rbp,rsp
ENDIF
        .setframe rbp,StkSizeLocal2

; Create the symbols for current function
        ValStackSizeTotal CATSTR <Prefix>,<StackSizeTotal>
        ValStackSizeTotal=StackSizeTotal

        ValStackSizeLocal1 CATSTR <Prefix>,<StackSizeLocal1>
        ValStackSizeLocal1=StkSizeLocal1

        ValStackSizeLocal2 CATSTR <Prefix>,<StackSizeLocal2>
        ValStackSizeLocal2=StkSizeLocal2

        ValNameOffsetHomeRCX CATSTR <Prefix>,<OffsetHomeRCX>
        ValNameOffsetHomeRCX=StkSizeLocal1+NumPushReg*8+StackPad+8

        ValNameOffsetHomeRDX CATSTR <Prefix>,<OffsetHomeRDX>
        ValNameOffsetHomeRDX=StkSizeLocal1+NumPushReg*8+StackPad+16

        ValNameOffsetHomeR8 CATSTR <Prefix>,<OffsetHomeR8>
        ValNameOffsetHomeR8=StkSizeLocal1+NumPushReg*8+StackPad+24

        ValNameOffsetHomeR9 CATSTR <Prefix>,<OffsetHomeR9>
        ValNameOffsetHomeR9=StkSizeLocal1+NumPushReg*8+StackPad+32

        ValNameOffsetStackArgs CATSTR <Prefix>,<OffsetStackArgs>
        ValNameOffsetStackArgs=StkSizeLocal1+NumPushReg*8+StackPad+40

        ValNameOffsetSaveXmmRegs CATSTR <Prefix>,<OffsetSaveXmmRegs>
        ValNameOffsetSaveXmmRegs=StkSizeLocal2

        ValNameOffsetDeleteFrame CATSTR <Prefix>,<OffsetDeleteFrame>
        ValNameOffsetDeleteFrame=StkSizeLocal1+StackPad
        endm

;-------------------------------------------------------------------------
; _EndProlog
;
; Description:  The following macro generates the .endprolog directive.
;-------------------------------------------------------------------------

_EndProlog  macro
        .endprolog
        endm

;-------------------------------------------------------------------------
; _DeleteFrame
;
; Description:  The following macro generates code that de-allocate a
;               stack frame previously created using _CreateFrame.
;
; Macro Parameters: Rnv1 - Rnv7     Non-volatile registers to restore
;-------------------------------------------------------------------------

_DeleteFrame macro Rnv1,Rnv2,Rnv3,Rnv4,Rnv5,Rnv6,Rnv7

IF (ValNameOffsetDeleteFrame GT 0)
        lea rsp,[rbp+ValNameOffsetDeleteFrame]
ELSE
        mov rsp,rbp
ENDIF

IFNB <Rnv7>
        pop Rnv7
ENDIF
IFNB <Rnv6>
        pop Rnv6
ENDIF
IFNB <Rnv5>
        pop Rnv5
ENDIF
IFNB <Rnv4>
        pop Rnv4
ENDIF
IFNB <Rnv3>
        pop Rnv3
ENDIF
IFNB <Rnv2>
        pop Rnv2
ENDIF
IFNB <Rnv1>
        pop Rnv1
ENDIF
        pop rbp
        endm

;-------------------------------------------------------------------------
; _SaveXmmRegs
;
; Description:  The following macro generates code that saves the
;               specified non-volatile registers to the local save area.
;
; Macro Parameters: Rnv1 - Rnv7     Non-volatile XMM registers to save.
;-------------------------------------------------------------------------

_SaveXmmRegs    macro Rnv0,Rnv1,Rnv2,Rnv3,Rnv4,Rnv5,Rnv6,Rnv7,Rnv8,Rnv9

NUMSAVEXMM = 0

IFNB <Rnv0>
        movdqa xmmword ptr [rbp-ValNameOffsetSaveXmmRegs],Rnv0
        .savexmm128 Rnv0,0
        NUMSAVEXMM = NUMSAVEXMM + 1
ENDIF
IFNB <Rnv1>
        movdqa xmmword ptr [rbp-ValNameOffsetSaveXmmRegs+16],Rnv1
        .savexmm128 Rnv1,16
        NUMSAVEXMM = NUMSAVEXMM + 1
ENDIF
IFNB <Rnv2>
        movdqa xmmword ptr [rbp-ValNameOffsetSaveXmmRegs+32],Rnv2
        .savexmm128 Rnv2,32
        NUMSAVEXMM = NUMSAVEXMM + 1
ENDIF
IFNB <Rnv3>
        movdqa xmmword ptr [rbp-ValNameOffsetSaveXmmRegs+48],Rnv3
        .savexmm128 Rnv3,48
        NUMSAVEXMM = NUMSAVEXMM + 1
ENDIF
IFNB <Rnv4>
        movdqa xmmword ptr [rbp-ValNameOffsetSaveXmmRegs+64],Rnv4
        .savexmm128 Rnv4,64
        NUMSAVEXMM = NUMSAVEXMM + 1
ENDIF
IFNB <Rnv5>
        movdqa xmmword ptr [rbp-ValNameOffsetSaveXmmRegs+80],Rnv5
        .savexmm128 Rnv5,80
        NUMSAVEXMM = NUMSAVEXMM + 1
ENDIF
IFNB <Rnv6>
        movdqa xmmword ptr [rbp-ValNameOffsetSaveXmmRegs+96],Rnv6
        .savexmm128 Rnv6,96
        NUMSAVEXMM = NUMSAVEXMM + 1
ENDIF
IFNB <Rnv7>
        movdqa xmmword ptr [rbp-ValNameOffsetSaveXmmRegs+112],Rnv7
        .savexmm128 Rnv7,112
        NUMSAVEXMM = NUMSAVEXMM + 1
ENDIF
IFNB <Rnv8>
        movdqa xmmword ptr [rbp-ValNameOffsetSaveXmmRegs+128],Rnv8
        .savexmm128 Rnv8,128
        NUMSAVEXMM = NUMSAVEXMM + 1
ENDIF
IFNB <Rnv9>
        movdqa xmmword ptr [rbp-ValNameOffsetSaveXmmRegs+144],Rnv9
        .savexmm128 Rnv9,144
        NUMSAVEXMM = NUMSAVEXMM + 1
ENDIF

; Make sure the stack frame was created with enough space to save
; all of the specified XMM registers.
.errnz (NUMSAVEXMM * 16) GT ValStackSizeLocal2, <XMM register save area is too small>
        endm

;-------------------------------------------------------------------------
; _RestoreXmmRegs
;
; Description:  The following macro generates code that restores the
;               specified non-volatile registers from the local save area.
;
; Macro Parameters: Rnv1 - Rnv7     Non-volatile XMM registers to restore.
;-------------------------------------------------------------------------

_RestoreXmmRegs macro Rnv0,Rnv1,Rnv2,Rnv3,Rnv4,Rnv5,Rnv6,Rnv7,Rnv8,Rnv9

IFNB <Rnv0>
        movdqa rnv0,xmmword ptr [rbp-ValNameOffsetSaveXmmRegs]
ENDIF
IFNB <Rnv1>
        movdqa rnv1,xmmword ptr [rbp-ValNameOffsetSaveXmmRegs+16]
ENDIF
IFNB <Rnv2>
        movdqa rnv2,xmmword ptr [rbp-ValNameOffsetSaveXmmRegs+32]
ENDIF
IFNB <Rnv3>
        movdqa rnv3,xmmword ptr [rbp-ValNameOffsetSaveXmmRegs+48]
ENDIF
IFNB <Rnv4>
        movdqa rnv4,xmmword ptr [rbp-ValNameOffsetSaveXmmRegs+64]
ENDIF
IFNB <Rnv5>
        movdqa rnv5,xmmword ptr [rbp-ValNameOffsetSaveXmmRegs+80]
ENDIF
IFNB <Rnv6>
        movdqa rnv6,xmmword ptr [rbp-ValNameOffsetSaveXmmRegs+96]
ENDIF
IFNB <Rnv7>
        movdqa rnv7,xmmword ptr [rbp-ValNameOffsetSaveXmmRegs+112]
ENDIF
IFNB <Rnv8>
        movdqa rnv8,xmmword ptr [rbp-ValNameOffsetSaveXmmRegs+128]
ENDIF
IFNB <Rnv9>
        movdqa rnv9,xmmword ptr [rbp-ValNameOffsetSaveXmmRegs+144]
ENDIF
        endm

        .list
        .listmacro
