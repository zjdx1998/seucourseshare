.8086
.MODEL SMALL  
.DATA   
ORG 100H
ARR BYTE 85,73,92,66,91,98,52,87,83,68
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
START:  MOV  AX,@DATA
        MOV  DS,AX
        
        MOV CX,10      
        
   SORT:DEC CX
        LEA SI,ARR
        MOV BX,1
        CMP CX,0
        JE N
  SSORT:CMP BX,CX
        JA  SORT
        MOV AL,[SI]
        CMP [SI+1],AL
        JA  SWAP
      R:INC SI
        INC BX
        JMP SSORT        
        
   SWAP:MOV DL,[SI+1]
        MOV [SI],DL
        MOV [SI+1],AL
        JMP R
        
      N:LEA SI,ARR
        MOV CX,10
        
 OUTPUT:MOV BL,10
        MOV AL,[SI]
        MOV AH,0
        DIV BL
        MOV DH,AH
        MOV AH,0   
        DIV BL
        MOV DL,AH
        ADD DL,30H
        MOV AH,02H
        INT 21H
        MOV DL,DH
        ADD DL,30H
        MOV AH,02H
        INT 21H
        CALL ENTER
        INC SI
        LOOP OUTPUT          
                
        MOV AH,4CH     
        INT 21H 
        
END  START