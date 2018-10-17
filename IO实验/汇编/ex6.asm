.8086
.MODEL SMALL  
.DATA   
ORG 100H
STA BYTE '$',3 DUP(?)
.CODE  
DAC PROC
     
    SUB AL,30H
    MOV AH,0
    MOV BX,AX
    MOV AX,1
    
    FACT:MUL BX
    DEC BX
    CMP BX,0
    JNZ FACT
    
    LEA SI,STA+1
    MOV BX,10
    
    SAV:DIV BX
    MOV [SI],DL
    INC SI    
    MOV DX,0
    CMP AX,0
    JNZ SAV
    
    DEC SI
    STD
    
    OUTPUT:LODSB  
    CMP AL,'$'
    JZ  EXIT 
    ADD AL,30H    
    MOV DL,AL
    MOV AH,02H
    INT 21H
    JMP OUTPUT
    
    EXIT:RET
DAC ENDP

START:  MOV  AX,@DATA
        MOV  DS,AX
                
        MOV  AH,01H
        INT  21H;输入字符
        
        PUSH AX
        
        MOV AH,02H   
        MOV DL,0AH
        INT  21H;输出换行  
        
        MOV DL,0DH   
        MOV AH,02H
        INT 21H;输出回车 
        
        POP AX
        
        CALL DAC
        
        MOV AH,4CH     
        INT 21H 
END  START