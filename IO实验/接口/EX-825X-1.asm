.8086
.MODEL SMALL  
.DATA  
COUNT BYTE 0H
.CODE  
START:  MOV AX,@DATA
        MOV DS,AX
        
        ;Init 8254        
        ;C0
        MOV AL,00110111B
        MOV DX,226H
        OUT DX,AL
        MOV AX,1000H
        MOV DX,220H
        OUT DX,AL
        MOV AL,AH
        OUT DX,AL
        
        ;C1
        MOV AL,01110111B
        MOV DX,226H
        OUT DX,AL
        MOV AX,1000H
        MOV DX,222H
        OUT DX,AL
        MOV AL,AH
        OUT DX,AL
        
        ;Init 8255
        MOV AL,10000000B
        MOV DX,206H
        OUT DX,AL
        MOV AL,10000001B
        MOV DX,200H
        OUT DX,AL
        
        ;Init 8259
        MOV AL,00010011B
        MOV DX,210H
        OUT DX,AL
        MOV AL,00001000B
        MOV DX,212H
        OUT DX,AL
        MOV AL,01H
        OUT DX,AL
        
        ;Init ÖÐ¶ÏÏòÁ¿±í
        CLI
        MOV AX,0
        MOV ES,AX
        MOV SI,0
        MOV CX,255
LOP:    MOV AX,OFFSET INT0
        MOV ES:[SI],AX
        MOV AX,SEG INT0
        MOV ES:[SI+2],AX        
        ADD SI,4
        LOOP LOP
        
        MOV AX,OFFSET INT0
        MOV ES:[SI],AX
        MOV AX,SEG INT0
        MOV ES:[SI+2],AX
        
        MOV AX,@DATA
        MOV DS,AX
        STI
        JMP $
        
        ;ÖÐ¶Ï·þÎñ³ÌÐò
INT0 PROC FAR
        PUSH SI
        PUSH AX
        PUSH DX
        PUSH DS
        MOV AX,@DATA
        MOV DS,AX
        INC COUNT
        MOV BL,COUNT
        CMP BL,10
        JZ  S1
        CMP BL,11
        JZ  S2
        CMP BL,12
        JZ  S1
        CMP BL,13
        JZ  S2
        CMP BL,14
        JZ  S3
        CMP BL,24
        JZ  S4
        CMP BL,25
        JZ  S5
        CMP BL,26
        JZ  S4
        CMP BL,27
        JZ  S5
        CMP BL,28
        JZ  S6
        JMP EXIT
        
        ;×ó»ÆµÆÁÁ
S1:     MOV AL,01000001B
        MOV DX,200H
        OUT DX,AL
        JMP EXIT
        
        ;×ó»ÆµÆ°µ
S2:     MOV AL,00000001B
        MOV DX,200H
        OUT DX,AL
        JMP EXIT
        
        ;×óºìÓÒÂÌ
S3:     MOV AL,00100100B
        MOV DX,200H
        OUT DX,AL
        JMP EXIT
        
        ;ÓÒ»ÆµÆÁÁ
S4:     MOV AL,00100010B
        MOV DX,200H
        OUT DX,AL
        JMP EXIT
      
        ;ÓÒ»ÆµÆ°µ
S5:     MOV AL,00100000B
        MOV DX,200H
        OUT DX,AL
        JMP EXIT  
        
        ;×óÂÌÓÒºì
S6:     MOV AL,10000001B
        MOV DX,200H
        OUT DX,AL
        MOV BL,0
        MOV COUNT,BL
        JMP EXIT        

EXIT:   MOV AL,20H
        MOV DX,210H
        OUT DX,AL                    
        POP DS
        POP DX
        POP AX
        POP SI              
        IRET
        
INT0 ENDP        
        
END  START