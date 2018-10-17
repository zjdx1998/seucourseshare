.8086
.MODEL SMALL  
.DATA   
ORG 100H
BUFF BYTE 30 DUP(?)
MSG BYTE 'You must input 0~9, or q or Q','$'
.CODE  
ENTER   PROC
        PUSH AX
        MOV DL,0AH
        MOV AH,02H
        INT 21H
        MOV DL,0DH
        MOV AH,02H
        INT 21H
        POP AX
        RET
ENTER   ENDP
        
PRINT   PROC
        PUSH CX
        SUB AL,30H        
        MOV CX,8
        MOV BL,AL        
P:      MOV DL,'0'
        SHL BL,1
        JNC NOCARRY
        INC DL
NOCARRY:
        MOV AH,02H
        INT 21H
        LOOP P
        POP CX        
        RET
PRINT   ENDP

START:  MOV AX,@DATA
        MOV DS,AX
        MOV CX,0
        LEA SI,BUFF
        
READ:   MOV AH,01H
        INT 21H
        CALL ENTER
        CMP AL,'Q'
        JE  DONE
        CMP AL,'q'
        JE  DONE
        CMP AL,'0'
        JB ERROR
        CMP AL,'9'
        JA  ERROR
        
        MOV [SI],AL
        INC SI
        
        CALL PRINT
        CALL ENTER
        LOOP READ
       
        
ERROR:  LEA DX,MSG
        MOV AH,09H
        INT 21H
        CALL ENTER
        JMP READ
        
DONE:   MOV AH,4CH
        INT 21H 
        
END  START