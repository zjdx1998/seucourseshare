.8086
.MODEL SMALL  
.DATA   
ORG 100H
ARR BYTE 'ABCDFGHIJ','$'
N BYTE 'B'
.CODE  
START:  MOV AX,@DATA
        MOV DS,AX
        LEA SI,ARR

FIND:   MOV AL,[SI]                
        CMP AL,N
        JZ  DELETE
        JA  NFIND
        INC SI
        LOOP FIND
        
NFIND:  MOV AH,0
        DEC AH
        JMP DONE
        
DELETE: INC SI
        MOV AL,[SI]
        MOV [SI-1],AL
        CMP AL,'$'
        JNZ  DELETE
        
        ADD AL,0
        
DONE:   MOV AH,4CH
        INT 21H 
        
END  START