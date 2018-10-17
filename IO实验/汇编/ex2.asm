.8086
.MODEL SMALL  
.DATA  
TABLE DB 0,1,4,9,16,25,36,49,64,81
XX DB 6
YY DB ?
.CODE  
START:
        MOV AX,@DATA
        MOV DS,AX
        
        MOV AL,XX
        MOV BX,OFFSET TABLE
        XLAT
        
        MOV AL,4
        XLAT
        MOV YY,AL
        
        MOV AH,4CH     
        INT 21H 
END  START