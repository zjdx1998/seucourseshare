.8086
.MODEL SMALL  
.DATA   
ORG 100H
.CODE  
START:  MOV AX,@DATA
        MOV DS,AX
        
        MOV AL,00010001B
        MOV DX,226H
        OUT DX,AL
        
        MOV AL,03H
        MOV DX,220H
        OUT DX,AL
        
END  START