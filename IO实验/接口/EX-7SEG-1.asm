.8086
.MODEL SMALL  
.DATA  
ARR BYTE 3FH,06H,5BH,4FH,66H,6DH,7DH,07H,7FH,6FH,77H,7CH,39H,5EH,79H,71H
.CODE  
START:  MOV AX,@DATA
        MOV DS,AX
        
        MOV AL,10000001B;CONTROL WORD
        MOV DX,206H
        OUT DX,AL
        
LOP:    MOV DX,204H
        IN  AL,DX
        MOV AH,0
        MOV DL,16
        DIV DL
        MOV AL,AH            
        LEA BX,ARR
        XLAT
        
        MOV DX,200H
        OUT DX,AL              
        JMP LOP
        
END  START