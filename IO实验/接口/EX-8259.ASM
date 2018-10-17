.MODEL SMALL
.8086
.stack
.data
come byte 00000000B   ; 计算中断到来次数的变量
.code
start:
    mov ax,@data
    mov ds,ax
    cli
    mov ax,0        ; init interrupt vector TABLE
    mov es,ax
    mov si,0         ; 由于模拟器的错误，所有中断向量需要指向同一个地址
    mov cx,255
l:  mov ax,offset int0
    mov es:[si],ax
    mov ax,seg int0
    mov es:[si+2],ax
    add si,4
    loop l
    mov ax,offset int0
    mov es:[si],ax
    mov ax,seg int0
    mov es:[si+2],ax
    mov ax,@data
    mov ds,ax
    mov al,00010011b  ;  init  8259
    mov dx,210h
    out dx,al         ; ICW1
    mov al,60h
    mov dx,212h
    out dx,al         ; ICW2
    mov ax,00000001h
    out dx,al         ; ICW4
    mov ax,0
    out dx,al         ; OCW1
    sti
    jmp $
int0  proc
    push ax
    push dx
    push ds
    mov ax,@data
    mov ds,ax   
    mov al, come     ; 中断到来，计数器+1
    inc al
    mov come, al
    mov al,20h       ; 发EOI命令
    mov dx,210h
    out dx,al
    pop ds
    pop dx
    pop ax
    iret
int0 endp
END start
