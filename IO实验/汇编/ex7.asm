.8086
.MODEL SMALL  
EXTRA SEGMENT
TABLE WORD 7,1,4,9,16,25,36,49
EXTRA ENDS
.CODE
START:  MOV AX,@DATA
        MOV DS,AX
        MOV BX,EXTRA
        MOV ES,BX 
        
        MOV AX,36
        LEA DI,ES:TABLE
        MOV DX,ES:[DI]
        SHL DX,1       
        MOV CX,2
        
   FIND:MOV BX,CX
        ADD BX,DX
        LEA DI,ES:TABLE
        ADD DI,BX
        SHR BX,1
        SHR DI,1
        CMP AX,ES:[DI]        
        JZ  OK
        JA  RIGHT
        JB  LEFT
        
  COMPA:CMP CX,DX
        JA  FAIL
        JMP FIND
                
  RIGHT:MOV CX,BX
        ADD CX,2
        JMP COMPA
        
   LEFT:MOV DX,BX
        SUB DX,2
        JMP COMPA
        
     OK:MOV SI,0
        ADD SI,BX
        CLC
        JMP EXIT
        
   FAIL:STC
        
   EXIT:MOV AH,4CH     
        INT 21H 
        
END  START