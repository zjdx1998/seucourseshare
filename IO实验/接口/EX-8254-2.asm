.8086
.MODEL SMALL  
.DATA   
ORG 100H
.CODE  
START:  MOV AX,@DATA
        MOV DS,AX
        
        MOV AL,00110111B    ;C/T0 
        MOV DX,226H
        OUT DX,AL
        MOV AX,1000H
        MOV DX,220H
        OUT DX,AL
        MOV AL,AH
        OUT DX,AL
        
        MOV AL,01110111B    ;C/T1
        MOV DX,226H
        OUT DX,AL
        MOV AX,1000H
        MOV DX,222H
        OUT DX,AL
        MOV AL,AH
        OUT DX,AL
        
        MOV AL,10110111B    ;C/T2
        MOV DX,226H
        OUT DX,AL
        MOV AX,05H
        MOV DX,224H
        OUT DX,AL
        MOV AL,AH
        OUT DX,AL        
        
END  START